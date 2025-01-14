// Copyright (c) 2019 Marco Wang <m.aesophor@gmail.com>. All rights reserved.
#include "util/box2d/b2BodyBuilder.h"

using std::unique_ptr;

namespace vigilante {

b2BodyBuilder::b2BodyBuilder(b2World* world)
    : _world(world), _body(), _fixture(), _shape(), _userData() {}

b2BodyBuilder::b2BodyBuilder(b2Body* body)
    : _world(body->GetWorld()), _body(body), _fixture(), _shape(), _userData() {}

b2BodyBuilder::b2BodyBuilder(const b2BodyBuilder& bodyBuilder)
    : _world(bodyBuilder._world),
      _body(bodyBuilder._body),
      _fixture(bodyBuilder._fixture),
      _shape(bodyBuilder._shape.get()),
      _userData(bodyBuilder._userData) {}

b2BodyBuilder& b2BodyBuilder::operator= (const b2BodyBuilder& bodyBuilder) {
  _world = bodyBuilder._world;
  _body = bodyBuilder._body;
  _fixture = bodyBuilder._fixture;
  _shape = unique_ptr<b2Shape>(bodyBuilder._shape.get());
  _userData = bodyBuilder._userData;
  return *this;
}


b2BodyBuilder& b2BodyBuilder::type(b2BodyType bodyType) {
  _bdef.type = bodyType;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::position(float x, float y, float ppm) {
  _bdef.position.Set(x / ppm, y / ppm);
  return *this;
}

b2Body* b2BodyBuilder::buildBody() {
  _body = _world->CreateBody(&_bdef);
  return _body;
}


b2BodyBuilder& b2BodyBuilder::newRectangleFixture(float hw, float hh, float ppm) {
  b2PolygonShape* shape = new b2PolygonShape();
  shape->SetAsBox(hw / ppm, hh / ppm);
  _shape = unique_ptr<b2PolygonShape>(shape);

  _fdef.shape = shape;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::newPolygonFixture(const b2Vec2* vertices, size_t count, float ppm) {
  b2PolygonShape* shape = new b2PolygonShape();
  b2Vec2 scaledVertices[count];
  for (size_t i = 0; i < count; i++) {
    scaledVertices[i] = {vertices[i].x / ppm, vertices[i].y / ppm};
  }
  shape->Set(scaledVertices, count);
  _shape = unique_ptr<b2PolygonShape>(shape);

  _fdef.shape = shape;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::newPolylineFixture(const b2Vec2* vertices, size_t count, float ppm) {
  b2ChainShape* shape = new b2ChainShape();
  b2Vec2 scaledVertices[count];
  for (size_t i = 0; i < count; i++) {
    scaledVertices[i] = {vertices[i].x / ppm, vertices[i].y / ppm};
  }
  shape->CreateChain(scaledVertices, count);
  _shape = unique_ptr<b2ChainShape>(shape);
  
  _fdef.shape = shape;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::newEdgeShapeFixture(const b2Vec2& vertex1, const b2Vec2& vertex2, float ppm) {
  b2EdgeShape* shape = new b2EdgeShape();
  b2Vec2 scaledVertex1 = {vertex1.x / ppm, vertex1.y / ppm};
  b2Vec2 scaledVertex2 = {vertex2.x / ppm, vertex2.y / ppm};
  shape->Set(scaledVertex1, scaledVertex2);

  _shape = unique_ptr<b2EdgeShape>(shape);
  _fdef.shape = shape;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::newCircleFixture(const b2Vec2& centerPos, int radius, float ppm) {
  b2CircleShape* shape = new b2CircleShape();
  shape->m_p.Set(centerPos.x / ppm, centerPos.y / ppm);
  shape->m_radius = radius / ppm;

  _shape = unique_ptr<b2CircleShape>(shape);
  _fdef.shape = shape;
  return *this;
}


b2BodyBuilder& b2BodyBuilder::categoryBits(short categoryBits) {
  _fdef.filter.categoryBits = categoryBits;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::maskBits(short maskBits) {
  _fdef.filter.maskBits = maskBits;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::setSensor(bool isSensor) {
  _fdef.isSensor = isSensor;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::friction(float friction) {
  _fdef.friction = friction;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::restitution(float restitution) {
  _fdef.restitution = restitution;
  return *this;
}

b2BodyBuilder& b2BodyBuilder::setUserData(void* userData) {
  _userData = userData;
  return *this;
}

b2Fixture* b2BodyBuilder::buildFixture() {
  _fixture = _body->CreateFixture(&_fdef);
  _fixture->SetUserData(_userData);
  _fdef = b2FixtureDef(); // Clear _fdef data
  return _fixture;
}

} // namespace vigilante
