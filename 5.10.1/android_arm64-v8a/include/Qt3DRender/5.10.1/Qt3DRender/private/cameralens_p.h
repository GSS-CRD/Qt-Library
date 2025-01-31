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

#ifndef QT3DRENDER_RENDER_CAMERALENS_H
#define QT3DRENDER_RENDER_CAMERALENS_H

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
#include <Qt3DCore/private/qnodecommand_p.h>
#include <Qt3DCore/private/matrix4x4_p.h>
#include <QRectF>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

class EntityManager;
class CameraManager;
class Sphere;

class CameraLensFunctor : public Qt3DCore::QBackendNodeMapper
{
public:
    explicit CameraLensFunctor(AbstractRenderer *renderer, QRenderAspect *renderAspect);
    Qt3DCore::QBackendNode *create(const Qt3DCore::QNodeCreatedChangeBasePtr &change) const Q_DECL_OVERRIDE;
    Qt3DCore::QBackendNode *get(Qt3DCore::QNodeId id) const Q_DECL_OVERRIDE;
    void destroy(Qt3DCore::QNodeId id) const Q_DECL_OVERRIDE;

private:
    CameraManager *m_manager;
    AbstractRenderer *m_renderer;
    QRenderAspect *m_renderAspect;
};

class QT3DRENDERSHARED_PRIVATE_EXPORT CameraLens : public BackendNode
{
public:
    CameraLens();
    ~CameraLens();
    void cleanup();

    void setRenderAspect(QRenderAspect* renderAspect);

    Matrix4x4 viewMatrix(const Matrix4x4 &worldTransform);

    void setProjection(const Matrix4x4 &projection);
    inline Matrix4x4 projection() const { return m_projection; }

    void setExposure(float exposure);
    inline float exposure() const { return m_exposure; }

    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;
    void notifySceneBoundingVolume(const Sphere &sphere, Qt3DCore::QNodeCommand::CommandId commandId);

    static bool viewMatrixForCamera(EntityManager *manager, Qt3DCore::QNodeId cameraId,
                                    Matrix4x4 &viewMatrix, Matrix4x4 &projectionMatrix);

private:
    void initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change) Q_DECL_FINAL;
    void computeSceneBoundingVolume(Qt3DCore::QNodeId entityId,
                                    Qt3DCore::QNodeId cameraId,
                                    Qt3DCore::QNodeCommand::CommandId commandId);

    QRenderAspect *m_renderAspect;
    Qt3DCore::QNodeCommand::CommandId m_pendingViewAllCommand;
    Matrix4x4 m_projection;
    float m_exposure;
};

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_RENDER_CAMERALENS_H
