#include <iostream>
#include<fstream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

class Image {
public:
	struct Header
	{
		char idLength;
		char colorMapType;
		char dataTypeCode;
		short colorMapOrigin;
		short colorMapLength;
		char colorMapDepth;
		short xOrigin;
		short yOrigin;
		short width;
		short height;
		char bitsPerPixel;
		char imageDescriptor;
	};
	struct Pixel {
		unsigned char redVal;
		unsigned char greenVal;
		unsigned char blueVal;
		Pixel() {
			redVal = 0;
			greenVal = 0;
			blueVal = 0;
		}
	};
	
	vector<Pixel> pixels;
	Header header;
};

unsigned int totalScore = 0;

void ReadingData(string fileName, Image &image1) {
	
	ifstream input(fileName, ios_base::binary);
	if (input.is_open()) {
		input.read((char*)&image1.header.idLength, 1);
		input.read((char*)&image1.header.colorMapType, 1);
		input.read((char*)&image1.header.dataTypeCode, 1);
		input.read((char*)&image1.header.colorMapOrigin, sizeof(image1.header.colorMapOrigin));
		input.read((char*)&image1.header.colorMapLength, 2);
		input.read((char*)&image1.header.colorMapDepth, 1);
		input.read((char*)&image1.header.xOrigin, 2);
		input.read((char*)&image1.header.yOrigin, 2);

		input.read((char*)&image1.header.width, 2);
		input.read((char*)&image1.header.height, 2);
		input.read((char*)&image1.header.bitsPerPixel, 1);
		input.read((char*)&image1.header.imageDescriptor, 1);
		//.seekg(whatever, 18);

		int numPixels = image1.header.width * image1.header.height;

		for (unsigned int i = 0; i < numPixels; i++) {
			Image::Pixel currentPixel;
			input.read((char*)&currentPixel.blueVal, 1);
			input.read((char*)&currentPixel.greenVal, 1);
			input.read((char*)&currentPixel.redVal, 1);

			image1.pixels.push_back(currentPixel);
		}

		input.close();
	}
}
void WritingData(const Image &image3, string fileName) {
	ofstream output("output/"+fileName, ios_base::binary);
	//Picuter.header has header info
	//Picute.pixels has pixels
	if (output.is_open()) {
		output.write((char*)&image3.header.idLength, 1);
		output.write((char*)&image3.header.colorMapType, 1);
		output.write((char*)&image3.header.dataTypeCode, 1);

		output.write((char*)&image3.header.colorMapOrigin, 2);
		output.write((char*)&image3.header.colorMapLength, 2);
		output.write((char*)&image3.header.colorMapDepth, 1);

		output.write((char*)&image3.header.xOrigin, 2);
		output.write((char*)&image3.header.yOrigin, 2);
		output.write((char*)&image3.header.width, 2);
		output.write((char*)&image3.header.height, 2);

		output.write((char*)&image3.header.bitsPerPixel, 1);
		output.write((char*)&image3.header.imageDescriptor, 1);

		for (int i = 0; i < image3.pixels.size(); i++) {
			Image::Pixel tempPixel = image3.pixels[i];

			output.write((char*)&tempPixel.blueVal, 1);
			output.write((char*)&tempPixel.greenVal, 1);
			output.write((char*)&tempPixel.redVal, 1);
		}

		output.close();
	}
	
}

/***Tasks***/
bool TestingResults(const Image &result, string fileName, string exampleName) {
	WritingData(result, fileName);

	Image example;
	exampleName = "examples/"+exampleName;
	ReadingData( exampleName, example);
	
	int count = 0;
	int index = 0;
	for (unsigned int i = 0; i < example.pixels.size(); i++) {
		if (result.pixels[i].blueVal != example.pixels[i].blueVal || result.pixels[i].greenVal != example.pixels[i].greenVal || result.pixels[i].redVal != example.pixels[i].redVal) {
			index = i;
			count++;
		}
	}
	if (count == 0) {
		cout << "All tests passed! Yayyy!!!" << endl;
		cout << endl;
		return true;
	}else{
		cout << count << " tests failed :(" << endl;
		
	}
	return false;
}

void MultiplyingPixels(const vector<Image::Pixel>  &topLayer, const vector<Image::Pixel>  &bottomLayer, vector<Image::Pixel> &resultingPixels) {
	
	for (unsigned int i = 0; i < topLayer.size(); i++) {
		Image::Pixel pixelin;
		pixelin.blueVal = ((topLayer[i].blueVal / 255.0) * (bottomLayer[i].blueVal / 255.0))*255.0 + 0.5f;
		
		pixelin.greenVal = ((topLayer[i].greenVal/255.0) * (bottomLayer[i].greenVal / 255.0))*255.0 + 0.5f;

		pixelin.redVal = ((topLayer[i].redVal / 255.0) * (bottomLayer[i].redVal / 255.0))*255.0 + 0.5f;

		resultingPixels.push_back(pixelin);
	}
}
void ScreenningPixels(vector<Image::Pixel> &topLayer, vector<Image::Pixel> &bottomLayer, vector<Image::Pixel> &resultingPixels) {
	for (unsigned int i = 0; i < topLayer.size(); i++) {
		Image::Pixel pixelin;
		pixelin.blueVal = 255 -(unsigned char) ((((255 - topLayer[i].blueVal) / 255.0) * ((255 - bottomLayer[i].blueVal) / 255.0))*255.0 + 0.5f);

		pixelin.greenVal = 255 - (unsigned char)((((255 - topLayer[i].greenVal) / 255.0) * ((255 - bottomLayer[i].greenVal) / 255.0))*255.0 + 0.5f);

		pixelin.redVal = 255 - (unsigned char)((((255 - topLayer[i].redVal) / 255.0) * ((255 - bottomLayer[i].redVal) / 255.0))*255.0 + 0.5f);

		resultingPixels.push_back(pixelin);
	}
}
void OverlayingPixels(vector<Image::Pixel> &topLayer, vector<Image::Pixel> &bottomLayer, vector<Image::Pixel> &resultingPixels) {
	unsigned int count = 0;
	double red, blue, green;

	for (unsigned int i = 0; i < bottomLayer.size(); i++) {
		Image::Pixel pixelin;

		if ((float)bottomLayer[i].blueVal / 255 <= 0.5) {
			blue = ((topLayer[i].blueVal / 255.0) * (bottomLayer[i].blueVal / 255.0));
			if ((blue * 2) > 1) {
				pixelin.blueVal = 255;
			}
			else {
				pixelin.blueVal = (blue * 2)*255.0 + 0.5f;
			}
		}
		else {
			blue = (((255 - topLayer[i].blueVal) / 255.0) * ((255 - bottomLayer[i].blueVal) / 255.0));
			if ((blue * 2) > 1) {
				pixelin.blueVal = 0;
			}
			else {
				blue *= 2;
				pixelin.blueVal = 255 - (unsigned char)(blue*255.0 + 0.5f);
			}
		}

		if ((float)bottomLayer[i].greenVal / 255 <= 0.5) {
			green = ((topLayer[i].greenVal / 255.0) * (bottomLayer[i].greenVal / 255.0));
			if ((green * 2) > 1) {
				pixelin.greenVal = 255;
			}
			else {
				pixelin.greenVal = (green * 2)*255.0 + 0.5f;
			}
		}
		else {
			green = (((255 - topLayer[i].greenVal) / 255.0) * ((255 - bottomLayer[i].greenVal) / 255.0));
			if ((green * 2) > 1) {
				pixelin.greenVal = 0;
			}
			else {
				green *= 2;
				pixelin.greenVal = 255 - (unsigned char)(green*255.0 + 0.5f);
			}
		}

		if ((float)bottomLayer[i].redVal / 255 <= 0.5) {
			red = ((topLayer[i].redVal / 255.0) * (bottomLayer[i].redVal / 255.0));
			if ((red * 2) > 1) {
				pixelin.redVal = 255;
			}
			else {
				pixelin.redVal = (red * 2)*255.0 + 0.5f;
			}
		}
		else {
			red = (((255 - topLayer[i].redVal) / 255.0) * ((255 - bottomLayer[i].redVal) / 255.0));
			if ((red * 2) > 1) {
				pixelin.redVal = 0;
			}
			else {
				red *= 2;
				pixelin.redVal = 255 - (unsigned char)(red*255.0 + 0.5f);
			}
		}


		resultingPixels.push_back(pixelin);
	}	
}

void SubstractingPixels(const Image &bottomLayer, const Image &topLayer, vector<Image::Pixel> &resultingPixels) {
	int newBlue;
	int newGreen;
	int newRed;	
	for (unsigned int i = 0; i < topLayer.pixels.size(); i++) {
		Image::Pixel pixelin;
		newBlue = topLayer.pixels[i].blueVal - bottomLayer.pixels[i].blueVal;
		if (newBlue < 0) {
			pixelin.blueVal = 0;
		}
		else if (newBlue > 255) {
			pixelin.blueVal = 255;
		}
		else {
			pixelin.blueVal = newBlue;
		}
		newGreen = topLayer.pixels[i].greenVal - bottomLayer.pixels[i].greenVal;
		if (newGreen < 0) {
			pixelin.greenVal = 0;
		}
		else if (newGreen > 255) {
			pixelin.greenVal = 255;
		}
		else {
			pixelin.greenVal = newGreen;
		}
		newRed = topLayer.pixels[i].redVal - bottomLayer.pixels[i].redVal;
		if (newRed < 0) {
			pixelin.redVal = 0;
		}
		else if (newRed > 255) {
			pixelin.redVal = 255;
		}
		else {
			pixelin.redVal = newRed;
		}

		resultingPixels.push_back(pixelin);
	}
}

void printScore() {
	cout << "Your current score is: " << totalScore << "/110" << endl;
	cout << endl;
}

void imageCollage(const Image &bottomLeft, const Image &bottomRight, const Image &topLeft, const Image &topRight, vector<Image::Pixel> &resultingPixels) {

	for (unsigned int i = 0; i < bottomLeft.header.height; i++) {
		for (unsigned int j = i*bottomLeft.header.width; j < (i*bottomLeft.header.width +bottomLeft.header.width); j++) {
			resultingPixels.push_back(bottomLeft.pixels[j]);
		}
		for (unsigned int j = i*bottomRight.header.width; j < (i*bottomRight.header.width +bottomRight.header.width); j++) {
			resultingPixels.push_back(bottomRight.pixels[j]);
		}
	}
	for (unsigned int i = 0; i < topLeft.header.height; i++) {
		for (unsigned int j = i*topLeft.header.width; j < (i*topLeft.header.width +topLeft.header.width); j++) {
			resultingPixels.push_back(topLeft.pixels[j]);
		}
		for (unsigned int j = i*topRight.header.width; j < (i*topRight.header.width +topRight.header.width); j++) {
			resultingPixels.push_back(topRight.pixels[j]);
		}
	}
}

int main(int argc, char *argv[])
{
	Image image1;
	Image image2;	
	Image image3;
	string filePath = "input/";
	bool passedTest = false;	
	vector<Image::Pixel> resultingPixels;

// Task 1
	cout << "*********Starting Task 1**********" << endl;
	cout << endl;

	ReadingData(filePath+"layer1.tga", image1);
	ReadingData(filePath+"pattern1.tga", image2);

	image3.header = image1.header;
	MultiplyingPixels(image1.pixels, image2.pixels, image3.pixels);
	
	passedTest = TestingResults(image3, "part1.tga", "EXAMPLE_part1.tga");
	if(passedTest){
		totalScore+=11;
	}
	printScore();
// Task 2
	cout << "*********Starting Task 2**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	ReadingData(filePath+"layer2.tga", image1);
	ReadingData(filePath+"car.tga", image2);
	
	image3.header = image1.header;
	SubstractingPixels(image1, image2, image3.pixels);
	passedTest = false;
	passedTest = TestingResults(image3, "part2.tga", "EXAMPLE_part2.tga");

	if(passedTest){
		totalScore+=11;
	}
	printScore();

// Task 3
	cout << "*********Starting Task 3**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	resultingPixels.clear();
	ReadingData(filePath+"layer1.tga", image1);
	ReadingData(filePath+"pattern2.tga", image2);
	image3.header = image1.header;
	MultiplyingPixels(image1.pixels, image2.pixels, image3.pixels);

	image1.pixels.clear();
	ReadingData(filePath+"text.tga", image1);
	ScreenningPixels(image1.pixels, image3.pixels, resultingPixels);

	image3.pixels = resultingPixels;
	passedTest = false;
	passedTest = TestingResults(image3, "part3.tga", "EXAMPLE_part3.tga");

	if(passedTest){
		totalScore+=11;
	}
	printScore();

// Task 4
	cout << "*********Starting Task 4**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	resultingPixels.clear();
	ReadingData(filePath+"layer2.tga", image1);
	ReadingData(filePath+"circles.tga", image2);

	image3.header = image1.header;
	MultiplyingPixels(image1.pixels, image2.pixels, image3.pixels);
	
	image1.pixels.clear();
	ReadingData(filePath+"pattern2.tga", image1);
	SubstractingPixels(image1, image3, resultingPixels);

	image3.pixels = resultingPixels;
	passedTest = false;
	passedTest = TestingResults(image3, "part4.tga", "EXAMPLE_part4.tga");
	if(passedTest){
		totalScore+=11;
	}
	printScore();
// Task 5
	cout << "*********Starting Task 5**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	ReadingData(filePath+"layer1.tga", image1);
	ReadingData(filePath+"pattern1.tga", image2);

	image3.header = image1.header;
	OverlayingPixels(image1.pixels, image2.pixels, image3.pixels);
	passedTest = false;
	passedTest = TestingResults(image3, "part5.tga", "EXAMPLE_part5.tga");
	if(passedTest){
		totalScore+=11;
	}
	printScore();

// Task 6
	cout << "*********Starting Task 6**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	ReadingData(filePath+"car.tga", image3);
	int value;
	for (unsigned int i = 0; i < image3.pixels.size();i++) {
		value = image3.pixels[i].greenVal + 200;
		if (value > 255) {
			image3.pixels[i].greenVal = 255;
		}
		else {
			image3.pixels[i].greenVal = value;
		}
	}
	passedTest = false;
	passedTest = TestingResults(image3, "part6.tga", "EXAMPLE_part6.tga");

	if(passedTest){
		totalScore+=11;
	}
	printScore();
// Task 7
	cout << "*********Starting Task 7**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	ReadingData(filePath+"car.tga", image3);
	//float temp;
	for (unsigned int i = 0; i < image3.pixels.size();i++) {
		//Red channel by 4 and blue channel by 0
		if (image3.pixels[i].redVal != 0) {
			if ((image3.pixels[i].redVal * 4) > 255) {
				image3.pixels[i].redVal = 255;
			}
			else {
				image3.pixels[i].redVal *= 4;
			}
		}
		if (image3.pixels[i].blueVal != 0) {
			image3.pixels[i].blueVal *= 0;
		}
	}
	passedTest = false;
	passedTest = TestingResults(image3, "part7.tga", "EXAMPLE_part7.tga");

	if(passedTest){
		totalScore+=11;
	}	
	printScore();
// Task 8
	cout << "*********Starting Task 8**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	ReadingData(filePath+"car.tga", image1);		
	image2.header = image1.header;
	bool passedTest1, passedTest2, passedTest3 = false;

	for (unsigned int i = 0; i < image1.pixels.size(); i++) {
		image2.pixels.push_back(image1.pixels[i]);
		image1.pixels[i].blueVal = image1.pixels[i].redVal;
		image1.pixels[i].greenVal = image1.pixels[i].redVal;
	}
	passedTest1 = TestingResults(image1, "part8_r.tga", "EXAMPLE_part8_r.tga");
	
	image1.pixels.clear();

	for (unsigned int i = 0; i < image2.pixels.size(); i++) {
		image1.pixels.push_back(image2.pixels[i]);
		image2.pixels[i].redVal = image1.pixels[i].blueVal;
		image2.pixels[i].greenVal = image1.pixels[i].blueVal;
	}
	passedTest2 = TestingResults(image2, "part8_b.tga", "EXAMPLE_part8_b.tga");
	image2.pixels.clear();

	for (unsigned int i = 0; i < image1.pixels.size(); i++) {
		image2.pixels.push_back(image1.pixels[i]);
		image1.pixels[i].redVal = image2.pixels[i].greenVal;
		image1.pixels[i].blueVal = image2.pixels[i].greenVal;
	}
	passedTest3 = TestingResults(image1, "part8_g.tga", "EXAMPLE_part8_g.tga");
	image1.pixels.clear();

	if(passedTest1 && passedTest2 && passedTest3){
		totalScore+=11;
	}
	printScore();

// Task 9
	cout << "*********Starting Task 9**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	ReadingData(filePath+"layer_red.tga", image1);//layer red
	ReadingData(filePath+"layer_blue.tga", image2);//layer blue
	ReadingData(filePath+"layer_green.tga", image3);//layer green

	for (unsigned int i = 0; i < image1.pixels.size(); i++) {
		image1.pixels[i].blueVal = image2.pixels[i].blueVal;
		image1.pixels[i].greenVal = image3.pixels[i].greenVal;
		
	}
	passedTest = false; 
	passedTest = TestingResults(image1, "part9.tga", "EXAMPLE_part9.tga");
	if(passedTest){
		totalScore+=11;
	}
	printScore();
// Task 10
	cout << "*********Starting Task 10**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	resultingPixels.clear();
	ReadingData(filePath+"text2.tga", image3);

	for (unsigned int i = image3.pixels.size()-1; i < -1 ; i--) {			
		resultingPixels.push_back(image3.pixels[i]);
	}

	image3.pixels = resultingPixels;
	passedTest = false;
	passedTest = TestingResults(image3, "part10.tga", "EXAMPLE_part10.tga");

	if(passedTest){
		totalScore+=11;
	}
	printScore();

	// Task 11
	cout << "*********Starting Task 11**********" << endl;
	cout << endl;
	image1.pixels.clear();
	image2.pixels.clear();
	image3.pixels.clear();
	resultingPixels.clear();
	Image image4;
	Image image5;

	ReadingData(filePath+"text.tga", image1);
	ReadingData(filePath+"pattern1.tga", image2);
	ReadingData(filePath+"car.tga", image3);
	ReadingData(filePath+"circles.tga", image4);

	imageCollage(image1, image2, image3, image4, resultingPixels);

	image5.header = image1.header;
	image5.header.height = image1.header.height + image3.header.height;
	image5.header.width = image1.header.width + image2.header.width;
	image5.pixels = resultingPixels;
	passedTest = false;
	passedTest = TestingResults(image5, "extraCredit.tga", "EXAMPLE_extraCredit.tga");

	if(passedTest){
		totalScore+=5;
	}
	printScore();

};
