/*****************************************************************************/
/**
 *  @file   ClusterMappedParallelCoordinatesRenderer.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ClusterMappedParallelCoordinatesRenderer.h 287 2012-06-15 14:20:06Z naohisa.sakamoto $
 */
/*****************************************************************************/
#ifndef KVSOCEANVIS__PCS__CLUSTER_MAPPED_PARALLEL_COORDINATES_RENDERER_H_INCLUDE
#define KVSOCEANVIS__PCS__CLUSTER_MAPPED_PARALLEL_COORDINATES_RENDERER_H_INCLUDE

#include <kvs/RendererBase>
#include <kvs/ClassName>
#include <kvs/Module>
#include <kvs/ColorMap>

namespace kvsoceanvis
{

namespace pcs
{

class ObjectBase;
class Camera;
class Light;

/*===========================================================================*/
/**
 *  @brief  Clustered mapped parallel coordinates renderer class.
 */
/*===========================================================================*/
class ClusterMappedParallelCoordinatesRenderer : public kvs::RendererBase
{
    // Class name.
    kvsClassName( kvsoceanvis::pcs::ClusterMappedParallelCoordinatesRenderer );

    // Module information.
    kvsModuleCategory( Renderer );
    kvsModuleBaseClass( kvs::RendererBase );

protected:

    int m_top_margin; ///< top margin
    int m_bottom_margin; ///< bottom margin
    int m_left_margin; ///< left margin
    int m_right_margin; ///< right margin
    mutable bool m_enable_anti_aliasing; ///< flag for anti-aliasing (AA)
    mutable bool m_enable_multisample_anti_aliasing; ///< flag for multisample anti-aliasing (MSAA)
    size_t m_active_axis; ///< index of active axis
    kvs::UInt8 m_cluster_opacity; ///< cluster opacity
    kvs::Real32 m_cluster_edge_width; ///< cluster edge width
    kvs::ColorMap m_color_map; ///< color map

public:

    ClusterMappedParallelCoordinatesRenderer();

public:

    void setTopMargin( const int top_margin );
    void setBottomMargin( const int bottom_margin );
    void setLeftMargin( const int left_margin );
    void setRightMargin( const int right_margin );
    void enableAntiAliasing( const bool multisample = false ) const;
    void disableAntiAliasing() const;
    void setClusterOpacity( const kvs::UInt8 opacity );
    void setClusterEdgeWidth( const kvs::Real32 width );
    void setColorMap( const kvs::ColorMap& color_map );
    void selectAxis( const size_t index );

    int topMargin() const;
    int bottomMargin() const;
    int leftMargin() const;
    int rightMargin() const;
    size_t activeAxis() const;
    kvs::UInt8 clusterOpacity() const;
    kvs::Real32 clusterEdgeWidth() const;

public:

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );
};

} // end of namespace pcs

} // end of namespace kvsoceanvis

#endif // KVSOCEANVIS__PCS__CLUSTER_MAPPED_PARALLEL_COORDINATES_RENDERER_H_INCLUDE
