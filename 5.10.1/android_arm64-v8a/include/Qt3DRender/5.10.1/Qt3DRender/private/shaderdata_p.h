/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT3DRENDER_RENDER_SHADERDATA_P_H
#define QT3DRENDER_RENDER_SHADERDATA_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <Qt3DRender/private/backendnode_p.h>
#include <Qt3DRender/private/shadervariables_p.h>
#include <Qt3DRender/qshaderdata.h>
#include <QMutex>
#include <Qt3DCore/private/matrix4x4_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

class GraphicsContext;
class GLBuffer;
class NodeManagers;

class Q_AUTOTEST_EXPORT ShaderData : public BackendNode
{
public:
    enum TransformType {
        NoTransform = -1,
        ModelToEye = 0,
        ModelToWorld,
        ModelToWorldDirection
    };

    ShaderData();
    ~ShaderData();

    QHash<QString, QVariant> properties() const { return m_originalProperties; }

    // Called by FramePreparationJob
    void updateWorldTransform(const Matrix4x4 &worldMatrix);

    // Call by RenderViewJob
    void markDirty();

    TransformType propertyTransformType(const QString &name) const;
    QVariant getTransformedProperty(const QString &name, const Matrix4x4 &viewMatrix);

    // Called by FrameCleanupJob
    static void cleanup(NodeManagers *managers);

    void setManagers(NodeManagers *managers);

protected:
    void initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change) Q_DECL_OVERRIDE;
    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;

    PropertyReaderInterfacePtr m_propertyReader;

    // 1 to 1 match with frontend properties, modified only by sceneChangeEvent
    QHash<QString, QVariant> m_originalProperties;

    // Contains properties thar are of type ShaderData
    QHash<QString, QVariant> m_nestedShaderDataProperties;

    // Contains property that are defined like: postionTransformed: ModelToEye
    QHash<QString, TransformType> m_transformedProperties;


    QMutex m_mutex;
    static QVector<Qt3DCore::QNodeId> m_updatedShaderData;
    Matrix4x4 m_worldMatrix;
    Matrix4x4 m_viewMatrix;
    NodeManagers *m_managers;

    void clearUpdatedProperties();
    static ShaderData *lookupResource(NodeManagers *managers, Qt3DCore::QNodeId id);
    ShaderData *lookupResource(Qt3DCore::QNodeId id);

    friend class RenderShaderDataFunctor;
};

class RenderShaderDataFunctor : public Qt3DCore::QBackendNodeMapper
{
public:
    explicit RenderShaderDataFunctor(AbstractRenderer *renderer, NodeManagers *managers);

    Qt3DCore::QBackendNode *create(const Qt3DCore::QNodeCreatedChangeBasePtr &change) const Q_DECL_FINAL;
    Qt3DCore::QBackendNode *get(Qt3DCore::QNodeId id) const Q_DECL_FINAL;
    void destroy(Qt3DCore::QNodeId id) const Q_DECL_FINAL;

private:
    NodeManagers *m_managers;
    AbstractRenderer *m_renderer;
};

} // namespace Render

} // namespace Qt3DRender

QT_END_NAMESPACE

Q_DECLARE_METATYPE(Qt3DRender::Render::ShaderData*) // LCOV_EXCL_LINE

#endif // QT3DRENDER_RENDER_SHADERDATA_P_H
