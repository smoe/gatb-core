/*****************************************************************************
 *   GATB : Genome Assembly Tool Box                                         *
 *   Authors: [R.Chikhi, G.Rizk, E.Drezen]                                   *
 *   Based on Minia, Authors: [R.Chikhi, G.Rizk], CeCILL license             *
 *   Copyright (c) INRIA, CeCILL license, 2013                               *
 *****************************************************************************/

/** \file Hash16.hpp
 *  \date 01/03/2013
 *  \author edrezen
 *  \brief Container implementation
 */

#ifndef _GATB_CORE_TOOLS_COLLECTIONS_IMPL_HASH16_HPP_
#define _GATB_CORE_TOOLS_COLLECTIONS_IMPL_HASH16_HPP_

/********************************************************************************/

#include <gatb/tools/collections/api/Container.hpp>
#include <gatb/tools/collections/api/Bag.hpp>
#include <gatb/tools/misc/impl/Pool.hpp>
#include <gatb/system/impl/System.hpp>

#include <set>
#include <algorithm>
#include <cmath> // for log2f

/********************************************************************************/
namespace gatb          {
namespace core          {
namespace tools         {
namespace collections   {
namespace impl          {
/********************************************************************************/

template <typename Item> class Hash16
{
protected:

    /** Shortcuts. */
    typedef typename misc::impl::Pool<Item>::cell        cell;
    typedef typename misc::impl::Pool<Item>::cell_ptr_t  cell_ptr_t;

    cell_ptr_t * datah;

    misc::impl::Pool<Item> storage;
    u_int64_t mask ;

    u_int64_t tai;
    u_int64_t nb_elem;
    u_int64_t max_nb_elem;

    /** Shortcut */
    system::IMemory& _memory;

public:

    /** */
    Hash16 (size_t sizeMB) : datah(0), mask(0), tai(0), nb_elem(0), max_nb_elem(0), _memory(system::impl::System::memory())
    {
        int tai_Hash16 = std::max (
            (size_t) ceilf (log2f ((0.1*sizeMB*1024L*1024L)/sizeof(cell_ptr_t))),
            (size_t)1
        );

        /** We check that the provided size is ok. */
        if (tai_Hash16 > 32)  {  throw system::Exception ("Hash16: max size for this hash is 2^32, but ask for %d", tai_Hash16);   }

        tai         = (1LL << tai_Hash16);
        mask        = tai-1 ;
        max_nb_elem = 0.8*sizeMB*1024LL*1024LL /sizeof(cell);
        datah       = (cell_ptr_t *) _memory.malloc( tai * sizeof(cell_ptr_t));  //create hashtable

        _memory.memset (datah,0, tai * sizeof(cell_ptr_t));
    }

    /** */
    ~Hash16()
    {
        _memory.free(datah);
    }

    /** */
    void clear ()
    {
        storage.clear ();
        nb_elem=0;
        _memory.memset (datah,0, tai * sizeof(cell_ptr_t));
    }

    /** */
    void insert (Item graine, int value)
    {
        unsigned int clef ;
        cell* cell_ptr    = 0;
        cell* newcell_ptr = 0;

        cell_ptr_t  newcell_internal_ptr;

        clef = (unsigned int) (hash(graine) & mask);

        cell_ptr = storage.internal_ptr_to_cell_pointer (datah[clef]);

        while(cell_ptr != NULL &&  cell_ptr->graine != graine)
        {
            cell_ptr = storage.internal_ptr_to_cell_pointer (cell_ptr->suiv);
        }
        if (cell_ptr==NULL) //graine non trouvee , insertion au debut
        {
            newcell_internal_ptr = storage.allocate_cell();
            newcell_ptr          = storage.internal_ptr_to_cell_pointer(newcell_internal_ptr);
            newcell_ptr->val     = value;
            newcell_ptr->graine  = graine;
            newcell_ptr->suiv    = datah[clef];
            datah[clef]          = newcell_internal_ptr;
            nb_elem++;
        }
        else
        {
            cell_ptr->val=value;  // graine trouvee
        }
    }

    /** */
    void insert (Item graine)
    {
        unsigned int clef ;
        cell* cell_ptr, *newcell_ptr;
        cell_ptr_t  newcell_internal_ptr;

        clef = (unsigned int) hash(graine) & mask;

        cell_ptr = storage.internal_ptr_to_cell_pointer(datah[clef]);

        while(cell_ptr != NULL &&  cell_ptr->graine != graine)
        {
            cell_ptr = storage.internal_ptr_to_cell_pointer (cell_ptr->suiv);
        }

        if (cell_ptr==NULL) //graine non trouvee , insertion au debut
        {
            newcell_internal_ptr = storage.allocate_cell();

            newcell_ptr         = storage.internal_ptr_to_cell_pointer(newcell_internal_ptr);
            newcell_ptr->val    = 1;
            newcell_ptr->graine = graine;
            newcell_ptr->suiv   = datah[clef];

            datah[clef] = newcell_internal_ptr;

            nb_elem++;
        }
        else
        {
            (cell_ptr->val)++;  // graine trouvee
        }
    }

    /** */
    int get( Item graine, int * val)
    {
        unsigned int clef ;
        cell* cell_ptr;

        clef = (unsigned int) hash (graine) & mask;

        cell_ptr = storage.internal_ptr_to_cell_pointer(datah[clef]);
        while(cell_ptr != NULL &&  cell_ptr->graine != graine)
        {
            cell_ptr = storage.internal_ptr_to_cell_pointer(cell_ptr->suiv);
        }

        if (cell_ptr==NULL)
        {
            return 0;
        }
        else
        {
            if (val != NULL)  {  *val = cell_ptr->val;  }
            return 1;
        }
    }

    /** */
    bool contains (Item graine)
    {
        return get (graine,NULL);
    }

    /** */
    int remove (Item graine, int * val)
    {
        unsigned int clef ;
        cell* cell_ptr;
        cell_ptr_t * cellprec_ptr;

        clef = (unsigned int) hash (graine) & mask;

        cell_ptr = storage.internal_ptr_to_cell_pointer(datah[clef]);
        cellprec_ptr = & (datah[clef]);

        while(cell_ptr != NULL &&  cell_ptr->graine != graine)
        {
            cellprec_ptr = & (cell_ptr->suiv);
            cell_ptr = storage.internal_ptr_to_cell_pointer(cell_ptr->suiv);
        }

        if (cell_ptr==NULL)
        {
            if (val != NULL)  {   *val = 0;  }
            return 0;

        }
        else
        {
            if (val != NULL)  {    *val = cell_ptr->val;  }
            //delete the cell :
            *cellprec_ptr = cell_ptr->suiv ;

            return 1;
        }
    }

    /** */
    u_int64_t size ()  { return nb_elem; }

    /** */
    u_int64_t getMaxNbItems ()  { return max_nb_elem; }
};

/********************************************************************************/
} } } } } /* end of namespaces. */
/********************************************************************************/

#endif /* _GATB_CORE_TOOLS_COLLECTIONS_IMPL_HASH16_HPP_ */
