#include "Curtain.h"



void Curtain::update(ChannelsReceiver &channels)
{
	if (!bActive) return;
	drowsiness = channels.delta.getOSCSample() + channels.theta.getOSCSample();
	maxDrowsiness = channels.delta.getHistoricalMax() + channels.theta.getHistoricalMax();
	strokeWidth = 1;
	//the more drowsiness the more strands
	strands = ofMap(fabs(drowsiness), 0, maxDrowsiness, 200, 500);
	//the distance between strands , density, is lower as drowsiness grows

	float width = ofGetWidth(), height = ofGetHeight();
	// First we compute target values, and the interpolate with actual ones
	// in order to made curtain movement more organic
	float a1Initial_x, a1Initial_y, a2Initial_x, a2Initial_y;
	a1Initial_x = -width*0.2; // not changing
	a1Initial_y = ofMap(fabs(drowsiness), 0, maxDrowsiness, height*0.8, -height*0.2); // will move from this value downwards generating strands
	a2Initial_x = width*1.2; // not changing
	a2Initial_y = ofMap(fabs(drowsiness), 0, maxDrowsiness, height*1.2, height*0.2); // will move from this value downwards generating strands

	a1Initial.x = ofLerp(a1Initial.x, a1Initial_x, 0.2);
	a1Initial.y = ofLerp(a1Initial.y, a1Initial_y, 0.2);
	a2Initial.x = ofLerp(a2Initial.x, a2Initial_x, 0.2);
	a2Initial.y = ofLerp(a2Initial.y, a2Initial_y, 0.2);
	a1Final = a1Initial; a1Final.y += height*0.8;
	a2Final = a2Initial; a2Final.y += height*0.6;

}

void Curtain::draw()
{ 
	if (!bActive) return;
	ofNoFill();
	ofSetColor(strokeColor);
	ofSetLineWidth(strokeWidth);


	float width = ofGetWidth(), height = ofGetHeight();
	ofLog() << "drowsiness:" << drowsiness << " a1Initial:" << a1Initial << " a2Initial:" << a2Initial << " a1Final:" << a1Final << " a2Final" << a2Final;
	ofBeginShape();
	// ai will move from ai.y to ai.y+nStrand*dStrand
	ofPoint a1=a1Initial, a2=a2Initial, b1, b2; // anchor points  and bezier points 
	b2.x = width / 2;
	b1.x = a1Initial.x;
	for (float nStrand = 0; nStrand < strands; nStrand++) 	
	{
		// generating strand along y
		a1.y = mapExp(nStrand, 0, strands, a1Initial.y, a1Final.y, 3);
		a2.y = mapExp(nStrand, 0, strands, a2Initial.y, a2Final.y, 3);
		ofVertex(a1.x, a1.y);
		//b1 = (a1 + a1Final) / 2;
		//b2 = (a2 + a2Final) / 2;
		b2.y = a2.y;
		b1.y =(a1.y + a2.y)/2;
		ofBezierVertex(b1.x, b1.y, b2.x, b2.y, a2.x, a2.y); //last anchor point is the lower right corner
		ofNextContour(true);
	}
	ofEndShape();


}