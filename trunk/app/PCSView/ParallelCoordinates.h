/*****************************************************************************/
/**
 *  @file   ParallelCoordinates.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ParallelCoordinates.h 18 2011-12-18 17:00:59Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef PARALLEL_COORDINATES_H_INCLUDE
#define PARALLEL_COORDINATES_H_INCLUDE

#include "Command.h"


/*===========================================================================*/
/**
 *  @brief  ParallelCoordinates command class.
 */
/*===========================================================================*/
class ParallelCoordinates : public Command
{
public:

    DefineCommandBaseClass( Command );
    DefineCommandName( "ParallelCoordinates" );
    DefineCommandDescription( "Parallel coordinates" );

public:

    ParallelCoordinates( int argc, char** argv );

    int exec( void );
};

#endif // PARALLEL_COORDINATES_H_INCLUDE
