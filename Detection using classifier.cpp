#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include "math.h"
#include "tinyxml2.h"
#include "tinyxml2.cpp"
#include "fstream"
#include "opencv2/objdetect.hpp"

using namespace cv;
using namespace std;
using namespace tinyxml2;

int main()
{
	int ImageCount = 0; //image count

	ofstream outfile("test.txt");
	int x = 1;
	int c = 0, minX = 0, maxX = 0, minY = 0, maxY = 0, w = 0, h = 0;
	//initialization for extraction from xml file
	vector<int> Occupied;
	vector<Point> Center;
	vector<Point> SizeWH;
	vector<int> Angle;
	vector<int> Spaceid;
	vector<Point> Coordinates;

	//initialization for images
	int c1 = 0, c2 = 0, c3 = 0; //give max count for the changing variables in image name
	
			for (int t = 1; t <= 20; t++)
			{
				
				//read an image
				//string name = format("F:/MS Courses/Computer Vision/PKLot/Set/2012-09-13_%02d_%02d_%02d.jpg", t1, t2, t3);
				string name = format("F:/MS Courses/Computer Vision/PKLot/Test images/Img%d.jpg", t);
				Mat img = imread(name, CV_LOAD_IMAGE_COLOR);

				// check if image is loaded
				if (img.empty())
				{
					cerr << name << " can't be loaded!" << endl;
					continue;
				}

				// show result:
				//imshow("test", img);
				//waitKey(); // yes, you need the waitKey()
				

				//creating strings
				string t1a = to_string(t);
				//string t2a = to_string(t2);
				//string t3a = to_string(t3);

				/*if (t1 < 10)
					t1a = "0" + t1a;
				if (t2 < 10)
					t2a = "0" + t2a;
				if (t3 < 10)
					t3a = "0" + t3a;
					*/
				//XML read
				string xmlname = "F:/MS Courses/Computer Vision/PKLot/Test images/Img" + t1a +".xml";
				//string xmlname = "F:/MS Courses/Computer Vision/PKLot/parking1b/cloudy/2013-02-22/2013-02-22_17_45_12.xml";
				char *cstr = new char[xmlname.length() + 1];
				strcpy(cstr, xmlname.c_str());

				//Loading XML file with error handling
				XMLDocument doc;

				XMLError e = doc.LoadFile(cstr);
				delete[] cstr;
				if (e != XML_SUCCESS)
				{
					cout << "failed to load file" << endl;

				}
				else
				{

					//Extracting root node

					XMLNode * pRoot = doc.FirstChildElement("parking");
					if (pRoot == nullptr)
					{
						cout << "No root node" << endl;
					}


					//Accessing elements and storing them
					XMLElement * firstSpaceElement = pRoot->FirstChildElement("space");
					XMLElement * lastSpaceElement = pRoot->LastChildElement("space");
					XMLElement * nextSpaceElement = firstSpaceElement->NextSiblingElement("space");
					XMLElement * rotatedRect = firstSpaceElement->FirstChildElement("rotatedRect");
					XMLElement * centerpoints = rotatedRect->FirstChildElement("center");
					XMLElement * size = rotatedRect->FirstChildElement("size");
					XMLElement * angle = rotatedRect->FirstChildElement("angle");
					XMLElement * contour = firstSpaceElement->FirstChildElement("contour");
					XMLElement * point = contour->FirstChildElement("point");
					XMLElement * nextpoint = point->NextSiblingElement("point");

					//Storing necessary data for first space in vectors
					Spaceid.push_back(atoi(firstSpaceElement->Attribute("id")));
					Occupied.push_back(atoi(firstSpaceElement->Attribute("occupied")));
					Center.push_back(Point(atoi(centerpoints->Attribute("x")), atoi(centerpoints->Attribute("y"))));
					SizeWH.push_back(Point(atoi(size->Attribute("w")), atoi(size->Attribute("h"))));
					Angle.push_back(atoi(angle->Attribute("d")));

					Coordinates.push_back(Point(atoi(point->Attribute("x")), atoi(point->Attribute("y"))));
					Coordinates.push_back(Point(atoi(nextpoint->Attribute("x")), atoi(nextpoint->Attribute("y"))));
					nextpoint = nextpoint->NextSiblingElement("point");
					Coordinates.push_back(Point(atoi(nextpoint->Attribute("x")), atoi(nextpoint->Attribute("y"))));
					nextpoint = nextpoint->NextSiblingElement("point");
					Coordinates.push_back(Point(atoi(nextpoint->Attribute("x")), atoi(nextpoint->Attribute("y"))));

					//calculating number of spaces
					int SpaceNos = atoi(lastSpaceElement->Attribute("id"));


					for (int i = 1; i < SpaceNos; i++)
					{
						//Accessing the next space elements
						rotatedRect = nextSpaceElement->FirstChildElement("rotatedRect");
						centerpoints = rotatedRect->FirstChildElement("center");
						size = rotatedRect->FirstChildElement("size");
						angle = rotatedRect->FirstChildElement("angle");
						contour = nextSpaceElement->FirstChildElement("contour");
						point = contour->FirstChildElement("point");
						nextpoint = point->NextSiblingElement("point");

						//storing necessary data in vectors
						Spaceid.push_back(atoi(nextSpaceElement->Attribute("id")));

						if (nextSpaceElement->Attribute("occupied") != NULL)
							Occupied.push_back(atoi(nextSpaceElement->Attribute("occupied")));

						else
							Occupied.push_back(-1);


						Center.push_back(Point(atoi(centerpoints->Attribute("x")), atoi(centerpoints->Attribute("y"))));
						SizeWH.push_back(Point(atoi(size->Attribute("w")), atoi(size->Attribute("h"))));
						Angle.push_back(atoi(angle->Attribute("d")));
						Coordinates.push_back(Point(atoi(point->Attribute("x")), atoi(point->Attribute("y"))));
						Coordinates.push_back(Point(atoi(nextpoint->Attribute("x")), atoi(nextpoint->Attribute("y"))));
						nextpoint = nextpoint->NextSiblingElement("point");
						Coordinates.push_back(Point(atoi(nextpoint->Attribute("x")), atoi(nextpoint->Attribute("y"))));
						nextpoint = nextpoint->NextSiblingElement("point");
						Coordinates.push_back(Point(atoi(nextpoint->Attribute("x")), atoi(nextpoint->Attribute("y"))));


						//loading the next space
						nextSpaceElement = nextSpaceElement->NextSiblingElement("space");

					}
					/*
					for (int i = 0; i < SpaceNos; i++)
					{

					//displaying values
					cout << "Space ID:" << Spaceid[i] << endl;
					cout << "Occupied:" << Occupied[i] << endl;
					cout << "Center:" << Center[i] << endl;
					cout << "Size:" << SizeWH[i] << endl;
					cout << "Angle:" << Angle[i] << endl;
					cout << "-----------------------" << endl;

					}
					*/

					String car_cascade_name = "F:\\MS Courses\\Computer Vision\\PKLot\\Haar Cascade 19 stages.xml";
					CascadeClassifier car_cascade;
					Mat gray;
					vector<Rect> cars;

					if (!car_cascade.load(car_cascade_name)) { printf("--(!)Error loading face cascade\n"); return -1; };

					//frame = imread("F:\\MS Courses\\Computer Vision\\PKLot\\parking2\\cloudy\\2012-09-12\\2012-09-12_09_08_18.jpg");
					cvtColor(img, gray, CV_RGB2GRAY);
					car_cascade.detectMultiScale(gray, cars, 1.05, 20, 0, Size(40, 40), Size(60, 60));
					cout << cars.size() << endl;
					for (int i = 0; i < cars.size(); i++)
					{
						rectangle(img, Point(cars[i].x, cars[i].y), Point(cars[i].x + cars[i].width, cars[i].y + cars[i].height), Scalar(255, 0, 0), 2);	
					}
					

					int total_occupied = 0;
					for (vector<int>::iterator it = Occupied.begin();it != Occupied.end();++it)
						total_occupied += *it;
					float dist1, dist2, dist3, dist4, hypo;
					int TP_flag = 0, TP_count = 0, FP_flag = 0, FP_count = 0, TN_flag = 0, TN_count = 0, FN_flag = 0, FN_count = 0, k;
					//to store distance from center of rotated rect to each vertex

					//for every parking space, find the closest and possible car in it
					for (int i = 0; i < SpaceNos; i++)
					{
						k = i * 4;
						int x_coord = Center[i].x;
						int y_coord = Center[i].y;
						//circle(img, Point(x_coord, y_coord), 3, Scalar(0, 0, 255), -1, 8, 0);
						TP_flag = 0;
						FN_flag = 0;
						if (Occupied[i] == 1)
						{
							//int x_coord = Center[i].x;
							//int y_coord = Center[i].y;

							for (int j = 0; j < cars.size(); j++)
							{
								dist1 = sqrt(pow((cars[j].x - x_coord), 2) + pow((cars[j].y - y_coord), 2));
								dist2 = sqrt(pow((cars[j].x + cars[j].width - x_coord), 2) + pow((cars[j].y - y_coord), 2));
								dist3 = sqrt(pow((cars[j].x - x_coord), 2) + pow((cars[j].y + cars[j].height - y_coord), 2));
								dist4 = sqrt(pow((cars[j].x + cars[j].width - x_coord), 2) + pow((cars[j].y + cars[j].height - y_coord), 2));
								hypo = sqrt(pow(cars[j].width, 2) + pow(cars[j].height, 2));

								if (0.5*hypo > min(dist1, min(dist2, min(dist3, dist4))))
									TP_flag = 1;
								
							}
							if (TP_flag == 0)
								FN_flag = 1;
							if (TP_flag)
							{
								TP_count = TP_count + 1;
								//cout << "TP:" << Spaceid[i] << endl;
							}
							if (FN_flag)
							{
								FN_count = FN_count + 1;
								//cout << "FN:" << Spaceid[i] << endl;
							}
						}
						FP_flag = 0;
						TN_flag = 0;
						if (Occupied[i] == 0)
						{
							//int x_coord = Center[i].x;
							//int y_coord = Center[i].y;

							for (int j = 0; j < cars.size(); j++)
							{
								dist1 = sqrt(pow((cars[j].x - x_coord), 2) + pow((cars[j].y - y_coord), 2));
								dist2 = sqrt(pow((cars[j].x + cars[j].width - x_coord), 2) + pow((cars[j].y - y_coord), 2));
								dist3 = sqrt(pow((cars[j].x - x_coord), 2) + pow((cars[j].y + cars[j].height - y_coord), 2));
								dist4 = sqrt(pow((cars[j].x + cars[j].width - x_coord), 2) + pow((cars[j].y + cars[j].height - y_coord), 2));
								hypo = sqrt(pow(cars[j].width, 2) + pow(cars[j].height, 2));

								if (0.5*hypo > min(dist1, min(dist2, min(dist3, dist4))))
									FP_flag = 1;
								
							}
							if (FP_flag == 0)
								TN_flag = 1;
							if (FP_flag)
							{
								FP_count = FP_count + 1;
								//cout << "FP:" << Spaceid[i] << endl;
							}
							if (TN_flag)
							{
								TN_count = TN_count + 1;
								//cout << "TN:" << Spaceid[i] << endl;
							}
						}
					}

					/*for (int j = 0; j < cars.size(); j++)
						circle(img, Point(cars[j].x + 0.5*cars[j].width, cars[j].y + 0.5*cars[j].height), 3, Scalar(0, 255, 0), -1, 8, 0);
					*/
					
					

					//cout << "Spaces :" << SpaceNos << " TP:" << TP_count << " FP:" << FP_count << " TN:" << TN_count << " FN:" << FN_count << endl;
					//cout << "Accuracy:" << float(TP_count + TN_count) / float(TP_count + TN_count + FP_count + FN_count) << endl;
					outfile << t << " " << TP_count << " " << FP_count << " " << 100*float(TP_count + TN_count) / float(TP_count + TN_count + FP_count + FN_count) <<endl;
					string writename = format("detect%d.jpg", t);
					imwrite(writename, img);

				}

				Occupied.clear();
				Center.clear();
				SizeWH.clear();
				Angle.clear();
				Spaceid.clear();
				Coordinates.clear();
			}
			outfile.close();
}
