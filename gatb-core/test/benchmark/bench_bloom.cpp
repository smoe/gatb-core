//
//  bench_bloom.cpp
//  gatb_xcode
//
//  Created by Guillaume Rizk on 20/06/13.
//  Copyright (c) 2013 Guillaume Rizk. All rights reserved.
//

#include <gatb/system/impl/System.hpp>

#include <gatb/bank/impl/Bank.hpp>
#include <gatb/bank/impl/BankBinary.hpp>
#include <gatb/bank/impl/BankHelpers.hpp>

#include <gatb/kmer/impl/Model.hpp>

#include <gatb/tools/designpattern/impl/IteratorHelpers.hpp>
#include <gatb/tools/designpattern/impl/Command.hpp>

#include <gatb/tools/collections/impl/BagFile.hpp>
#include <gatb/tools/collections/impl/BagCache.hpp>
#include <gatb/tools/collections/impl/IteratorFile.hpp>
#include <gatb/tools/collections/impl/Bloom.hpp>

#include <gatb/tools/misc/impl/Progress.hpp>
#include <gatb/tools/misc/impl/Property.hpp>
#include <gatb/tools/misc/impl/TimeInfo.hpp>

#include <iostream>
#include <map>
#include <math.h>
#include <libgen.h>
#include <stdlib.h>
#include <math.h>

#include <gatb/tools/math/Integer.hpp>


// We use the required packages
using namespace std;
using namespace gatb::core::system;
using namespace gatb::core::system::impl;
using namespace gatb::core::bank;
using namespace gatb::core::bank::impl;
using namespace gatb::core::kmer;
using namespace gatb::core::kmer::impl;
using namespace gatb::core::tools::dp;
using namespace gatb::core::tools::dp::impl;
using namespace gatb::core::tools::misc;
using namespace gatb::core::tools::misc::impl;
using namespace gatb::core::tools::math;
using namespace gatb::core::tools::misc::impl;



using namespace gatb::core::tools::collections;
using namespace gatb::core::tools::collections::impl;

using namespace gatb::core::tools::math;

#define MAX_RANDOM 2147483648


uint64_t random64 ()
{
    uint64_t low, high,res;
    low = random();
    high = random();
    
    res = (high << 32) + low;
    return res;
}

int main (int argc, char* argv[])
{
    if (argc < 4)
    {
        cerr << "you must provide at least 2 arguments. Arguments are:" << endl;
        cerr << "   1) bloom size"  << endl;
        cerr << "   2) nb elems inserted"  << endl;
        cerr << "   3) nb hash funcs"  << endl;

        return EXIT_FAILURE;
    }
    
    TimeInfo            _timeInfo;

    Properties res;
    


    
    // We define the max size of a data line in the FASTA output file
    uint64_t bloomsize = atoll(argv[1]);
    uint64_t nelems = atoll(argv[2]);
    uint64_t nhash = atoll(argv[3]);
    uint64_t resu =0;
    double  ratio  = bloomsize / (double) nelems ;
    double  expected_FP  = pow(0.6185,ratio);
    double  measured_FP  = 0;

    int ideal_nb_hash =   (int)floorf (0.7*ratio);
    
     srandomdev(); 
    

    kmer_type start = random();
    kmer_type kmer_random, kmer_current;
    kmer_current = start;
    // We define a try/catch block in case some method fails (bad filename for instance)
    try
    {

        
        //    8589934592   bits for 1GB   //1 = 780903144
        /** We create a bloom with inserted solid kmers. */
        kmer_type kmertemp;
      //  Bloom<kmer_type>* bloom =  new Bloom<kmer_type> (bloomsize,nhash);
        BloomCacheCoherent<kmer_type>* bloom =  new BloomCacheCoherent<kmer_type> (bloomsize,nhash,12);

        _timeInfo.start ("Inserting N elements");

        for(int ii =0; ii<nelems; ii++)
        {
            bloom->insert(kmer_current);
            kmer_current = kmer_current +1;
        }
        _timeInfo.stop ("Inserting N elements");
        

        
        
        //we query the elements just inserted, ie only positive elements
        _timeInfo.start ("Query N elements");
        kmer_current = start;
        for(int ii =0; ii<nelems; ii++)
        {
            
            resu+=bloom->contains(kmer_current);
            kmer_current = kmer_current +1;

        }
        _timeInfo.stop ("Query N elements");
        
        
        
        //////////////////////// testing fp rate with random elements
        
        delete bloom;
       // bloom =  new Bloom<kmer_type> (bloomsize,nhash);
        bloom =  new BloomCacheCoherent<kmer_type> (bloomsize,nhash,12);

        //insert n randoms
        for(int ii = 0; ii<nelems; ii++)
        {
            kmer_random = random64();
            bloom->insert(kmer_random);
        }
        
        uint64_t ntrue = 0;
        uint64_t ntested = 10000000;

        for(int ii = 0; ii<ntested; ii++)
        {
            kmer_random = random64(); // we expect it not be in the bloom
            
            if (bloom->contains(kmer_random)) // FP
            {
                ntrue++;
            }
        }
        
        measured_FP =  ntrue / (double) ntested ;
        
        //printf("expected FP rate : %g \n",expected_FP);
        //printf("measured FP rate : %g \n",measured_FP);
        
        res.add (0, "Bloom tested", "");
        res.add (1, "Size T in bits ", "%lli",bloomsize);
        res.add (1, "Size T in MB ", "%lli",bloomsize/8LL/1024LL/1024LL);

        res.add (1, "N elems inserted  ", "%lli",nelems);
        res.add (1, "nb  hash funcs", "%i",nhash);
        res.add (1, "ratio bits/elem", "%g",bloomsize/(double) nelems);

        
        res.add (0, "Bloom perf", "");
        res.add (1, _timeInfo.getProperties("time"));
        
        res.add (0, "False positive rate", "");
        res.add (1, "ideal nb hash func would be ", "%i", ideal_nb_hash);
        res.add (1, "expected ideal FP rate ", "%g", expected_FP);
        res.add (1, "measured FP with T,N,nb", "%g", measured_FP);

        
        RawDumpPropertiesVisitor visit;
        res.accept (&visit);
    }
    
    catch (gatb::core::system::Exception& e)
    {
        cerr << "EXCEPTION: " << e.getMessage() << endl;
    }
    
    return EXIT_SUCCESS;
}

