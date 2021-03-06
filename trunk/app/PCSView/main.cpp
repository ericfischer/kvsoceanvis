/*****************************************************************************/
/**
 *  @file   main.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: main.cpp 29 2011-12-22 07:29:14Z naohisa.sakamoto $
 */
/*****************************************************************************/
#include <string>
#include <cstdlib>
#include <kvs/CommandLine>
#include "ParallelCoordinates.h"
#include "MultiBinnedParallelCoordinates.h"
#include "MultiDimensionalBinning.h"
#include "GrADS2Table.h"
#include "ClusteredParallelCoordinates.h"
#include "LinkedView.h"


namespace { Command* Cmd = NULL; }
namespace { void DelCmd( void ) { delete ::Cmd; } }
namespace { void DeleteAtExit( Command* cmd ) { ::Cmd = cmd; std::atexit( ::DelCmd ); } }

/*===========================================================================*/
/**
 *  @brief  Main function.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
int main( int argc, char** argv )
{
    // Command names.
    const std::string name[] = {
        ParallelCoordinates::CommandName(),
        MultiBinnedParallelCoordinates::CommandName(),
        MultiDimensionalBinning::CommandName(),
        GrADS2Table::CommandName(),
        ClusteredParallelCoordinates::CommandName(),
        LinkedView::CommandName()
    };

    // Command descriptions.
    const std::string desc[] = {
        ParallelCoordinates::CommandDescription(),
        MultiBinnedParallelCoordinates::CommandDescription(),
        MultiDimensionalBinning::CommandDescription(),
        GrADS2Table::CommandDescription(),
        ClusteredParallelCoordinates::CommandDescription(),
        LinkedView::CommandDescription()
    };

    // Parse command line argument.
    kvs::CommandLine commandline( argc, argv );
    commandline.addHelpOption();
    commandline.addOption( name[0], desc[0] + "." );
    commandline.addOption( name[1], desc[1] + "." );
    commandline.addOption( name[2], desc[2] + "." );
    commandline.addOption( name[3], desc[3] + "." );
    commandline.addOption( name[4], desc[4] + "." );
    commandline.addOption( name[5], desc[5] + "." );
    commandline.addValue( "input data file", false );
    if ( !commandline.read() ) return( false );

    // Create user specified command.
    Command* command = NULL;
    if ( commandline.hasOption( name[0] ) )
    {
        command = new ParallelCoordinates( argc, argv );
        if ( !command ) return( false );
    }
    else if ( commandline.hasOption( name[1] ) )
    {
        command = new MultiBinnedParallelCoordinates( argc, argv );
        if ( !command ) return( false );
    }
    else if ( commandline.hasOption( name[2] ) )
    {
        command = new MultiDimensionalBinning( argc, argv );
        if ( !command ) return( false );
    }
    else if ( commandline.hasOption( name[3] ) )
    {
        command = new GrADS2Table( argc, argv );
        if ( !command ) return( false );
    }
    else if ( commandline.hasOption( name[4] ) )
    {
        command = new ClusteredParallelCoordinates( argc, argv );
        if ( !command ) return( false );
    }
    else if ( commandline.hasOption( name[5] ) )
    {
        command = new LinkedView( argc, argv );
        if ( !command ) return( false );
    }
    else
    {
        commandline.showHelpMessage();
        return( false );
    }

    // Set the command to the exit function.
    DeleteAtExit( command );

    // Execute the command.
    return( command->exec() );
}
