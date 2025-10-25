#include <cmath>

#include "./physics_debug_system.h"

using namespace SDL2pp;

PhysicsDebugSystem::PhysicsDebugSystem(Renderer& renderer): renderer_(renderer) {}

void PhysicsDebugSystem::RenderWorld(b2WorldId worldId, float ppm) {
    b2BodyId bodyId = b2World_GetBodyList(worldId);

    while (b2Body_IsValid(bodyId)) {
        DrawBody(bodyId, ppm);
        bodyId = b2Body_GetNext(bodyId);
    }
}

void PhysicsDebugSystem::DrawBody(b2BodyId bodyId, float ppm) {
    b2ShapeId shapeId = b2Body_GetShapeList(bodyId);

    while (b2Shape_IsValid(shapeId)) {
        b2ShapeType type = b2Shape_GetType(shapeId);

        if (type == b2ShapeTypePolygon) {
            const b2Polygon* poly = b2Shape_GetPolygon(shapeId);
            DrawPolygon(poly->vertices, poly->count, ppm);
        }

        shapeId = b2Shape_GetNext(shapeId);
    }
}

void PhysicsDebugSystem::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, float ppm) {
    renderer_.SetDrawColor(0, 255, 0, 255);

    for (int i = 0; i < vertexCount; ++i) {
        b2Vec2 v1 = vertices[i];
        b2Vec2 v2 = vertices[(i + 1) % vertexCount];

        renderer_.DrawLine(v1.x * ppm + 400, 300 - v1.y * ppm, v2.x * ppm + 400, 300 - v2.y * ppm);
    }
}
