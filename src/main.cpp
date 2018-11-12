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
	cout << "A new .tga file has been created called " << fileName << endl;
}
string GetFileName() {
	cout << "Write the name of the file: ";
	string input = "";
	cin >> input;
	input += ".tga";
	cout << "The file name is: " << input << endl;
	return input;
}
void CheckingHeaders(const Image &topLayer, const Image &result) {
	cout << "Checking the header info:" << endl <<
		"Top layer id lenght: " << topLayer.header.idLength << "		Resulting picture: " << result.header.idLength << endl <<
		"Top layer width: " << topLayer.header.width << "		Resulting picture: " << result.header.width << endl <<
		"Top layer lenght: " << topLayer.header.height << "		Resulting picture: " << result.header.height << endl;
}
/***Tasks***/
void TestingResults(const Image &result, string fileName) {
	WritingData(result, fileName);
	cout << "********************" << endl;
	cout << "Testing resulting image!" << endl;
	Image example;
	string exampleName = "examples/"+GetFileName();
	cout << "Example filename: " <<exampleName << endl;
	ReadingData( exampleName, example);
	cout << "Starting to compare pixels..." << endl; cout << endl;
	int count = 0;
	int index = 0;
	cout << "Example pic size: "<<example.pixels.size() << endl;
	for (unsigned int i = 0; i < example.pixels.size(); i++) {
		if (result.pixels[i].blueVal != example.pixels[i].blueVal || result.pixels[i].greenVal != example.pixels[i].greenVal || result.pixels[i].redVal != example.pixels[i].redVal) {
			index = i;
			count++;
		}
	}
	if (count == 0) {
		cout << "All tests passed! Yayyy!!!" << endl;
		cout << endl;
	}
	else if (count == example.pixels.size()) {
		cout <<"All tests failed :(" << endl << "For example: " << endl;
		cout << "Test " << index << " failed" << endl <<
			"Created picture values: " <<
			"Blue: " << toascii(result.pixels[index].blueVal) <<
			" Green: " << toascii(result.pixels[index].greenVal) <<
			" Red: " << toascii(result.pixels[index].redVal) << endl <<
			"Example picture values: " <<
			"Blue: " << toascii(example.pixels[index].blueVal) <<
			" Green: " << toascii(example.pixels[index].greenVal) <<
			" Red: " << toascii(example.pixels[index].redVal) << endl;
		cout << "Test 0 failed" << endl <<
			"Created picture values: " <<
			"Blue: " << toascii(result.pixels[0].blueVal) <<
			" Green: " << toascii(result.pixels[0].greenVal) <<
			" Red: " << toascii(result.pixels[0].redVal) << endl <<
			"Example picture values: " <<
			"Blue: " << toascii(example.pixels[0].blueVal) <<
			" Green: " << toascii(example.pixels[0].greenVal) <<
			" Red: " << toascii(example.pixels[0].redVal) << endl;
		cout << endl;
	}else{
		cout << count << " tests failed :(" << endl << "For example: " << endl;
		cout << "Test " << index << " failed" << endl <<
			"Created picture values: " <<
			"Blue: " << toascii(result.pixels[index].blueVal) <<
			" Green: " << toascii(result.pixels[index].greenVal) <<
			" Red: " << toascii(result.pixels[index].redVal) << endl <<
			"Example picture values: " <<
			"Blue: " << toascii(example.pixels[index].blueVal) <<
			" Green: " << toascii(example.pixels[index].greenVal) <<
			" Red: " << toascii(example.pixels[index].redVal) << endl;
		cout << endl;
	}
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

		if (bottomLayer[i].blueVal / 255 <= 0.5) {
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

		if (bottomLayer[i].greenVal / 255 <= 0.5) {
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

		if (bottomLayer[i].redVal / 255 <= 0.5) {
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


		////Apply multiplication agorithm
		//if ((bottomLayer[i].blueVal/255 <= 0.5) || (bottomLayer[i].redVal/255 <= 0.5) || (bottomLayer[i].greenVal/255 <= 0.5)) {
		//	blue = ((topLayer[i].blueVal / 255.0) * (bottomLayer[i].blueVal / 255.0));
		//	green = ((topLayer[i].greenVal / 255.0) * (bottomLayer[i].greenVal / 255.0));
		//	red = ((topLayer[i].redVal / 255.0) * (bottomLayer[i].redVal / 255.0));
		//	/*pixelin.blueVal = ((topLayer[i].blueVal / 255.0) * (bottomLayer[i].blueVal / 255.0))*255.0 + 0.5f;
		//	pixelin.greenVal = ((topLayer[i].greenVal / 255.0) * (bottomLayer[i].greenVal / 255.0))*255.0 + 0.5f;
		//	pixelin.redVal = ((topLayer[i].redVal / 255.0) * (bottomLayer[i].redVal / 255.0))*255.0 + 0.5f;*/
		//	
		//	if ((red * 2) > 1) {
		//		pixelin.redVal = 255;
		//	}
		//	else {
		//		pixelin.redVal = (red * 2)*255.0 + 0.5f;
		//	}
		//	
		//	if ((blue * 2) > 1) {
		//		pixelin.blueVal = 255;
		//	}
		//	else {
		//		pixelin.blueVal = (blue*2)*255.0 + 0.5f;
		//	}
		//	
		//	if ((green * 2) > 1) {
		//		pixelin.greenVal = 255;
		//	}
		//	else {
		//		pixelin.greenVal = (green * 2)*255.0 + 0.5f;
		//	}
		//	
		//}
		//
		//else {	//Apply screenning agorithm	
		//	blue = (((255 - topLayer[i].blueVal) / 255.0) * ((255 - bottomLayer[i].blueVal) / 255.0));
		//	green = (((255 - topLayer[i].greenVal) / 255.0) * ((255 - bottomLayer[i].greenVal) / 255.0));
		//	red = (((255 - topLayer[i].redVal) / 255.0) * ((255 - bottomLayer[i].redVal) / 255.0));
		//	/*pixelin.blueVal =  (((255 - topLayer[i].blueVal) / 255.0) * ((255 - bottomLayer[i].blueVal) / 255.0))*255.0 + 0.5f;
		//	pixelin.greenVal = (((255 - topLayer[i].greenVal) / 255.0) * ((255 - bottomLayer[i].greenVal) / 255.0))*255.0 + 0.5f;
		//	pixelin.redVal = (((255 - topLayer[i].redVal) / 255.0) * ((255 - bottomLayer[i].redVal) / 255.0))*255.0 + 0.5f;*/
		//	
		//		if ((red * 2) > 1) {
		//			pixelin.redVal = 0;
		//		}
		//		else {
		//			red *= 2;
		//			pixelin.redVal = 255 -(unsigned char)(red*255.0 + 0.5f);
		//		}
		//	
		//	
		//		if ((blue * 2) > 1) {
		//			pixelin.blueVal = 0;
		//		}
		//		else {
		//			blue *= 2;
		//			pixelin.blueVal = 255 - (unsigned char)(blue*255.0 + 0.5f);
		//		}
		//	
		//	
		//		if ((green * 2) > 1) {
		//			green = 0;
		//		}
		//		else {
		//			green *= 2;
		//			pixelin.greenVal = 255 - (unsigned char)(green*255.0 + 0.5f);
		//		}
		//	
		//}
		//
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

int main(int argc, char *argv[])
{
	Image image1;
	Image image2;	
	Image image3;
	string filePath = "input/";
	
	vector<Image::Pixel> resultingPixels;

	cout << "Choose the task: " << endl <<
		"1. Multiply layer1(top) and pattern1(bottom)" << endl <<
		"2. Substract layer2(top) and car(bottom)" << endl <<
		"3. Multiply layer1(top) and pattern2(bottom) and Screen text(top) with previous result(bottom)" << endl <<
		"4. Multiply layer2(top) with circles(bottom) and Substract pattern2(top) with previous result(bottom)" << endl <<
		"5. Overlay layer1(top) with pattern1(bottom)" << endl <<
		"6. Add 200 to car's green channel" << endl <<
		"7. Load car and scale (multiply) the red channel by 4, and the blue channel by 0" << endl <<
		"8. Load car and write each channel to a separate file" << endl <<
		"9. Combine layer_red, layer_blue and layer_green” into one file" << endl <<
		"10. Rotate it text2 180 degrees" << endl;
	// unsigned int task = 0;
	// cin >> task;
	unsigned int task = stoi(argv[1]);
	switch (task)
	{
	case 1:
		ReadingData(filePath+argv[2], image1);
		ReadingData(filePath+argv[3], image2);

		image3.header = image1.header;
		MultiplyingPixels(image1.pixels, image2.pixels, image3.pixels);
		
		TestingResults(image3, "part1.tga");
		break;
	case 2:
		ReadingData(filePath+argv[2], image1);
		ReadingData(filePath+argv[3], image2);
		
		image3.header = image1.header;
		SubstractingPixels(image1, image2, image3.pixels);
		TestingResults(image3, "part2.tga");

		break;
	case 3:
		ReadingData(filePath+argv[2], image1);
		ReadingData(filePath+argv[3], image2);
		image3.header = image1.header;
		MultiplyingPixels(image1.pixels, image2.pixels, image3.pixels);

		image1.pixels.clear();
		cout << "**Third image to be loaded** " << endl;
		ReadingData(filePath+argv[4], image1);
		ScreenningPixels(image1.pixels, image3.pixels, resultingPixels);

		image3.pixels = resultingPixels;

		TestingResults(image3, "part3.tga");

		break;
	case 4:
		ReadingData(filePath+argv[2], image1);
		ReadingData(filePath+argv[3], image2);

		image3.header = image1.header;
		MultiplyingPixels(image1.pixels, image2.pixels, image3.pixels);
		
		image1.pixels.clear();
		cout << "**Third image to be loaded** " << endl;
		ReadingData(filePath+argv[4], image1);
		SubstractingPixels(image1, image3, resultingPixels);

		image3.pixels = resultingPixels;
		
		TestingResults(image3, "part4.tga");
		break;
	case 5:
		ReadingData(filePath+argv[2], image1);
		ReadingData(filePath+argv[3], image2);

		image3.header = image1.header;
		OverlayingPixels(image1.pixels, image2.pixels, image3.pixels);

		TestingResults(image3, "part5.tga");
		break;
	case 6:
		ReadingData(filePath+argv[2], image3);
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
		
		TestingResults(image3, "part6.tga");
		break;

	case 7:
		ReadingData(filePath+argv[2], image3);
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

		TestingResults(image3, "part7.tga");
		break;
		
	case 8:
		ReadingData(filePath+argv[2], image1);		
		image2.header = image1.header;

		for (unsigned int i = 0; i < image1.pixels.size(); i++) {
			image2.pixels.push_back(image1.pixels[i]);
			image1.pixels[i].blueVal = image1.pixels[i].redVal;
			image1.pixels[i].greenVal = image1.pixels[i].redVal;
		}
		TestingResults(image1, "part8_r.tga");
		image1.pixels.clear();

		for (unsigned int i = 0; i < image2.pixels.size(); i++) {
			image1.pixels.push_back(image2.pixels[i]);
			image2.pixels[i].redVal = image1.pixels[i].blueVal;
			image2.pixels[i].greenVal = image1.pixels[i].blueVal;
		}
		TestingResults(image2, "part8_b.tga");
		image2.pixels.clear();

		for (unsigned int i = 0; i < image1.pixels.size(); i++) {
			image2.pixels.push_back(image1.pixels[i]);
			image1.pixels[i].redVal = image2.pixels[i].greenVal;
			image1.pixels[i].blueVal = image2.pixels[i].greenVal;
		}
		TestingResults(image1, "part8_g.tga");
		image1.pixels.clear();


		break;

	case 9:
		ReadingData(filePath+argv[2], image1);//layer red
		ReadingData(filePath+argv[3], image2);//layer blue
		ReadingData(filePath+argv[4], image3);//layer green
	
		for (unsigned int i = 0; i < image1.pixels.size(); i++) {
			image1.pixels[i].blueVal = image2.pixels[i].blueVal;
			image1.pixels[i].greenVal = image3.pixels[i].greenVal;
			
		}

		TestingResults(image1, "part9.tga");
		break;

	default:
		ReadingData(filePath+argv[2], image3);

		for (unsigned int i = image3.pixels.size()-1; i < -1 ; i--) {			
			resultingPixels.push_back(image3.pixels[i]);
		}

		image3.pixels = resultingPixels;

		TestingResults(image3, "part10.tga");
		break;
	}
};

