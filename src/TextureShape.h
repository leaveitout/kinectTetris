//
//  TextureShape.h
//  example-ShapeTexturing
//
//  Created by Todd Vanderlin on 7/16/13.
//
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

class TextureShape {
  
public:
    ofImage    *    texturePtr;
    ofMesh          mesh;
    ofColor         color;
    ofxBox2dPolygon polyShape;
    
    TextureShape() {
        texturePtr = NULL;
    }
    void setup(ofxBox2d &world, float x, float y, float w, float h) 
	{
        color.setHex(0xffffff);
        
        mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_TRIANGLES);

		polyShape.addVertex(x,y);
		polyShape.addVertex(x+w,y);
		polyShape.addVertex(x+w,y+h);
		polyShape.addVertex(x,y+h);

		ofPoint nw, ne, se, sw;

		nw = ofPoint(0.0F, 0.0F);
		ne = ofPoint(1.0F, 0.0F);
		se = ofPoint(1.0F, 1.0F);
		sw = ofPoint(0.0F, 1.0F);

		addTexCoords(mesh, nw, ne, se, sw);
        
        polyShape.setPhysics(3.0, 0.53, 0.1);
    	polyShape.create(world.getWorld());
    }
    void setTexture(ofImage * texture) {
        texturePtr = texture;
        texturePtr->getTextureReference().setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        texturePtr->getTextureReference().setTextureWrap(GL_REPEAT, GL_REPEAT);
    }
    void draw() {
        
        mesh.clearVertices();
        vector<ofPoint> &pts = polyShape.getPoints();

		// TODO: get rid of ugly array order
		// Order found by experimentation
		addFace(mesh, pts[3], pts[0], pts[1], pts[2]);

        ofSetColor(color);
        texturePtr->bind();
        mesh.draw();
        texturePtr->unbind();
   
    }

private:
	/*
	 These functions are for adding quads and triangles to an ofMesh -- either
	 vertices, or texture coordinates.
	 */
	//--------------------------------------------------------------
	void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c) {
		mesh.addVertex(a);
		mesh.addVertex(b);
		mesh.addVertex(c);
	}

	//--------------------------------------------------------------
	void addFace(ofMesh& mesh, ofVec3f a, ofVec3f b, ofVec3f c, ofVec3f d) {
		addFace(mesh, a, b, c);
		addFace(mesh, a, c, d);
	}

	//--------------------------------------------------------------
	void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c) {
		mesh.addTexCoord(a);
		mesh.addTexCoord(b);
		mesh.addTexCoord(c);
	}

	//--------------------------------------------------------------
	void addTexCoords(ofMesh& mesh, ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d) {
		addTexCoords(mesh, a, b, c);
		addTexCoords(mesh, a, c, d);
	}
    
};