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

/** \mainpage GATB Core Documentation
 *
 ****************************************************************************************************
 ****************************************************************************************************
 * \section intro What is GATB ?
 *
 *  GATB means "Genome Assembly Tool Box".
 *
 *  It can be seen as software components providing services for assembling genomes of various kinds.
 *
 ****************************************************************************************************
 ****************************************************************************************************
 * \section purpose Purpose of the GATB core library
 *
 * The documentation you are reading is about the GATB core library.
 *
 * One purpose of this library is to provide an easy way to handle a central structure in the
 * GATB project: the <a href="http://en.wikipedia.org/wiki/De_Bruijn_graph">De Bruijn graph</a>
 *
 * More precisely, the intent is to provide means to build De Bruijn graphs with a low memory footprint,
 * which comes initially from the <a href="http://minia.genouest.org">minia</a> assembly tool.
 *
 *
 ****************************************************************************************************
 ****************************************************************************************************
 * \section struct Services provided by the GATB core library
 *
 * From the client point of view, the gatb::core package provides:
 *   - libraries that offer low level genomic operations, up to the De Bruign graph creation
 *   - tests of the libraries
 *   - snippets showing how to use the library
 *   - specific binaries that rely on the libraries
 *   - wrappers of the libraries services for several langages (java, python, ...)
 *
 * You will find here the code documentation for namespaces, classes, methods of the different
 * components that compose the <b>gatb::core</b> design.
 *
 ****************************************************************************************************
 ****************************************************************************************************
 * \section howto How can I use the GATB core library ?
 *
 * As a starting point, it is strongly recommended to have a look at \ref snippets_page
 *
 ****************************************************************************************************
 ****************************************************************************************************
 * \section other Other material
 *
 * You can also read the related pages:
 *      - \ref design_page
 *      - \ref tests_page
 */
