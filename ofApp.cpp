#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	ofDisableArbTex();

	this->frame_size = cv::Size(720, 720);
	this->rect_size = 120;
	for (int x = 0; x < this->frame_size.width; x += this->rect_size) {

		for (int y = 0; y < this->frame_size.height; y += this->rect_size) {

			auto image = make_unique<ofImage>();
			image->allocate(this->rect_size, this->rect_size, OF_IMAGE_COLOR);
			cv::Mat frame = cv::Mat(cv::Size(image->getWidth(), image->getHeight()), CV_MAKETYPE(CV_8UC3, image->getPixels().getNumChannels()), image->getPixels().getData(), 0);
			cv::Rect rect = cv::Rect(x, y, this->rect_size, this->rect_size);

			this->rect_images.push_back(move(image));
			this->cv_rects.push_back(rect);
			this->rect_frames.push_back(frame);
		}
	}

	cv::Mat pic = cv::imread("D:\\char.jpg");
	cv::resize(pic, this->frame, this->frame_size);
	cv::cvtColor(this->frame, this->frame, cv::COLOR_BGR2RGB);
	cv::flip(this->frame, this->frame, 0);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	for (int i = 0; i < this->rect_frames.size(); i++) {

		cv::Mat tmp_box_image(this->frame, this->cv_rects[i]);
		tmp_box_image.copyTo(this->rect_frames[i]);
		
		auto noise_value = ofNoise(this->cv_rects[i].x * 0.005, this->cv_rects[i].y * 0.005, ofGetFrameNum() * 0.002);
		int deg = 0;
		if (noise_value > 0.4 && noise_value < 0.6) {

			deg = ofMap(noise_value, 0.4, 0.6, 0, 360);
		}

		if (deg < 80) { deg = 0; }
		else if (deg < 90) { deg = ofMap(deg, 80, 90, 0, 90); }
		else if (deg < 170) { deg = 90; }
		else if (deg < 180) { deg = ofMap(deg, 170, 180, 90, 180); }
		else if (deg < 260) { deg = 180; }
		else if (deg < 270) { deg = ofMap(deg, 260, 270, 180, 270); }
		else if (deg < 350) { deg = 270; }
		else if (deg < 360) { deg = ofMap(deg, 350, 360, 270, 360); }

		ofPushMatrix();
		ofTranslate(this->cv_rects[i].x + this->rect_size * 0.5, this->frame_size.height - this->cv_rects[i].y - this->rect_size * 0.5, 0);
		ofRotate(deg);

		this->rect_images[i]->update();
		this->rect_images[i]->getTexture().bind();
		ofSetColor(255);
		ofFill();
		ofDrawBox(glm::vec3(), this->rect_size, this->rect_size, 1);
		this->rect_images[i]->unbind();

		ofSetColor(39);
		ofNoFill();
		ofDrawBox(glm::vec3(), this->rect_size, this->rect_size, 1);

		ofPopMatrix();
	}

}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}
