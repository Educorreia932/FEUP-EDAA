#include "LineShape.h"

#include <cmath>

using namespace std;
using namespace sf;

const float ArrowHead::widthFactor   = 4.0;
const float ArrowHead::lengthFactor  = 4.0;
const float ArrowHead::advanceFactor = 1.0;

LineShape::LineShape(const Vector2f& u, const Vector2f& v, float w):
    VertexArray(Quads),
    u(u),v(v),w(w)
{}
void LineShape::setFrom(const Vector2f& u){ this->u = u; }
const Vector2f& LineShape::getFrom() const{ return u; }
void LineShape::setTo(const Vector2f& v){ this->v = v; }
const Vector2f& LineShape::getTo() const{ return v; }
void LineShape::setWidth(float w){ this->w = w; }
float LineShape::getWidth() const{ return w; }

void LineShape::setFillColor(Color color){
    for(size_t i = 0; i < getVertexCount(); ++i){
        (*this)[i].color = color;
    }
}

void LineShape::append(const sf::Vertex &v){ VertexArray::append(v); }
void LineShape::append(const sf::VertexArray &v){
    for(size_t i = 0; i < v.getVertexCount(); ++i){
        append(v[i]);
    }
}

FullLineShape::FullLineShape(const Vector2f& u, const Vector2f& v, float w):
    LineShape(u,v,w)
{
    process();
}

void FullLineShape::setFrom (const Vector2f& u){ LineShape::setFrom (u); process(); }
void FullLineShape::setTo   (const Vector2f& v){ LineShape::setTo   (v); process(); }
void FullLineShape::setWidth(             float  w){ LineShape::setWidth(w); process(); }

void FullLineShape::process(){
    const Vector2f &u = getFrom();
    const Vector2f &v = getTo  ();
    Vector2f v_u = v-u;
    float magnitude = sqrt(v_u.x*v_u.x + v_u.y*v_u.y);
    v_u /= magnitude;

    Vector2f edgeV = v-u;
    Vector2f edgeNorm(-edgeV.y, edgeV.x);
    float magnitudeNorm = sqrt(edgeNorm.x*edgeNorm.x + edgeNorm.y*edgeNorm.y);
    edgeNorm /= magnitudeNorm;
    edgeNorm *= (getWidth()/2);

    resize(0);

    append(Vertex(u-edgeNorm));
    append(Vertex(u+edgeNorm));
    append(Vertex(v+edgeNorm));
    append(Vertex(v-edgeNorm));
}

DashedLineShape::DashedLineShape(const Vector2f& u, const Vector2f& v, float w):
    LineShape(u,v,w)
{
    process();
}

void DashedLineShape::setFrom (const Vector2f& u){ LineShape::setFrom (u); process(); }
void DashedLineShape::setTo   (const Vector2f& v){ LineShape::setTo   (v); process(); }
void DashedLineShape::setWidth(             float  w){ LineShape::setWidth(w); process(); }

void DashedLineShape::process(){
    float interDashesSpace = 4.0f*getWidth();
    const Vector2f &u = getFrom();
    const Vector2f &v = getTo  ();
    Vector2f v_u = v-u;
    float magnitude = sqrt(v_u.x*v_u.x + v_u.y*v_u.y);
    v_u /= magnitude;

    Vector2f edgeV = v-u;
    Vector2f edgeNorm(-edgeV.y, edgeV.x);
    float magnitudeNorm = sqrt(edgeNorm.x*edgeNorm.x + edgeNorm.y*edgeNorm.y);
    edgeNorm /= magnitudeNorm;
    edgeNorm *= (getWidth()/2);

    resize(0);

    // Most dashes
    int nDashes = (int)(magnitude/interDashesSpace);
    float nDashesFloat = (float) nDashes;
    for(int i = 0; i < nDashes; ++i){
        Vector2f u1 = u + v_u*interDashesSpace*(float(i)         );
        Vector2f v1 = u + v_u*interDashesSpace*(float(i)+dashFill);
        
        append(Vertex(u1-edgeNorm));
        append(Vertex(u1+edgeNorm));
        append(Vertex(v1+edgeNorm));
        append(Vertex(v1-edgeNorm));
    }

    // Last dash
    Vector2f u1 = u + v_u*    interDashesSpace*(nDashesFloat         )            ;
    Vector2f v1 = u + v_u*min(interDashesSpace*(nDashesFloat+dashFill), magnitude);
        
    append(Vertex(u1-edgeNorm));
    append(Vertex(u1+edgeNorm));
    append(Vertex(v1+edgeNorm));
    append(Vertex(v1-edgeNorm));
}


ArrowHead::ArrowHead(const Vector2f& u, const Vector2f& v, float w):
    LineShape(u,v,w)
{
    process();
}

void ArrowHead::setFrom (const Vector2f& u){ LineShape::setFrom (u); process(); }
void ArrowHead::setTo   (const Vector2f& v){ LineShape::setTo   (v); process(); }
void ArrowHead::setWidth(         float  w){ LineShape::setWidth(w); process(); }

void ArrowHead::process(){
    const Vector2f &u = getFrom();
    const Vector2f &v = getTo  ();
    Vector2f uvVec = v-u;
    Vector2f uvUnitVec = uvVec/sqrt(uvVec.x*uvVec.x + uvVec.y*uvVec.y); // unit vector from u to v
    Vector2f uvNormUnitVec(-uvUnitVec.y, uvUnitVec.x); // unit vector perpendicular to uvUnitVec

    resize(0);

    append(Vertex(v));
    append(Vertex(v));
    append(Vertex(v-(uvUnitVec*lengthFactor + uvNormUnitVec*widthFactor/2.0f)*getWidth()));
    append(Vertex(v-(uvUnitVec*(lengthFactor - advanceFactor))*getWidth()));
    
    append(Vertex(v));
    append(Vertex(v));
    append(Vertex(v-(uvUnitVec*lengthFactor - uvNormUnitVec*widthFactor/2.0f)*getWidth()));
    append(Vertex(v-(uvUnitVec*(lengthFactor - advanceFactor))*getWidth()));
}

sf::Vector2f ArrowHead::getLineConnection() const {
    const Vector2f &u = getFrom();
    const Vector2f &v = getTo  ();
    Vector2f uvVec = v-u;
    Vector2f uvUnitVec = uvVec/sqrt(uvVec.x*uvVec.x + uvVec.y*uvVec.y); // unit vector from u to v
    
    float lineLengthFactor = min(lengthFactor-advanceFactor, lengthFactor);
    return v - uvUnitVec*lineLengthFactor*getWidth();
}
