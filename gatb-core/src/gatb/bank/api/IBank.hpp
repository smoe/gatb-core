/*****************************************************************************
 *   GATB : Genome Assembly Tool Box                                         *
 *   Copyright (c) 2013                                                      *
 *                                                                           *
 *   GATB is free software; you can redistribute it and/or modify it under   *
 *   the CECILL version 2 License, that is compatible with the GNU General   *
 *   Public License                                                          *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 *   CECILL version 2 License for more details.                              *
 *****************************************************************************/

/** \file IBank.hpp
 *  \date 01/03/2013
 *  \author edrezen
 *  \brief Interface definition for genomic databases management
 */

#ifndef _GATB_CORE_BANK_IBANK_HPP_
#define _GATB_CORE_BANK_IBANK_HPP_

/********************************************************************************/

#include <gatb/tools/collections/api/Iterable.hpp>
#include <gatb/tools/collections/api/Bag.hpp>
#include <gatb/bank/api/Sequence.hpp>

/********************************************************************************/
namespace gatb      {
/** \brief Core package of the GATP project.
 *
 * The gatb::core package holds all the fundamental packages needed for writting
 * assembly algorithms.
 *
 * It holds some generic tools, like operating system abstraction, collections management or design patterns
 * concerns. It also holds recurrent needs as reading genomic banks, handling kmers and so on.
 */
namespace core      {
/** \brief Package for genomic databases management. */
namespace bank      {
/********************************************************************************/

/** \brief Interface for what we need to read genomic databases.
 *
 * The IBank interface is a essentially a short name for an Iterable over Sequence
 * instances.
 */
class IBank : public tools::collections::Iterable<Sequence>, public tools::collections::Bag<Sequence>
{
public:

    /** \copydoc tools::collections::Iterable::iterator */
    virtual tools::dp::Iterator<Sequence>* iterator () = 0;

    /** \copydoc tools::collections::Bag */
    virtual void insert (const Sequence& item) = 0;

    /** Return the size of the bank (comments + data)
     *
     * The returned value may be an approximation in some case. For instance, if we use
     * a zipped bank, an implementation may be not able to give accurate answer to the
     * size of the original file.
     *
     * \return the bank size.*/
    virtual u_int64_t getSize () = 0;

    /** Give an estimation of the sequences number in the bank.
     * \return the sequences number estimation. */
    virtual u_int64_t estimateNbSequences () = 0;

    /** Give an estimation of the maximal length of the sequences in the bank.
     * \return the maximum length estimation */
    virtual size_t estimateMaxSequenceLength () = 0;
};

/********************************************************************************/
} } } /* end of namespaces. */
/********************************************************************************/

#endif /* _GATB_CORE_BANK_IBANK_HPP_ */
