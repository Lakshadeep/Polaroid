#include "cv.h"
#include "highgui.h"
#include "math.h"
#include <iostream>
#include <cstdio>
#include "serialport.h"

#define PORTNAME "COM1"
#define RIGHT "1"
#define LEFT "2"
#define FRONT "3"
#define BACK "4"
#define UPL "8"
#define DOWNL "9"
#define STOP "5"
#define STOPL "0"



using namespace cv;
Mat original,original1,pattern,arena_calib,block_arena,island,arena_calib2;

int angle = 0;
char* Rotation;
void rotate_fn(int,void*);

char* Top;
char* Bottom;
char* Left;
char* Right;
void crop(int,void*);
void crop_pattern(int,void*);
void crop_island( int,void*);
int left=1,right=640,top=1,bottom=480;
int pleft,pright,ptop,pbottom;
int left_is,right_is,top_is,bottom_is;
int pattern_points[9][2];
int pattern_color[9];

char* lowerH;
char* lowerS;
char* lowerV;
char* upperH;
char* upperS;
char* upperV;

char* lowerHbb;
char* lowerSbb;
char* lowerVbb;
char* upperHbb;
char* upperSbb;
char* upperVbb;
void Threshold_bot_back(int,void*);
int lowerh_bot_back,lowers_bot_back,lowerv_bot_back,upperh_bot_back,uppers_bot_back,upperv_bot_back;
char* lowerHbf;
char* lowerSbf;
char* lowerVbf;
char* upperHbf;
char* upperSbf;
char* upperVbf;
void Threshold_bot_front(int,void*);
int lowerh_bot_front,lowers_bot_front,lowerv_bot_front,upperh_bot_front,uppers_bot_front,upperv_bot_front;

char* lowerHc1;
char* lowerSc1;
char* lowerVc1;
char* upperHc1;
char* upperSc1;
char* upperVc1;
void color1_detection(int, void* );
int lowerh_color1,lowers_color1,lowerv_color1,upperh_color1,uppers_color1,upperv_color1;
char* lowerHc2;
char* lowerSc2;
char* lowerVc2;
char* upperHc2;
char* upperSc2;
char* upperVc2;
void color2_detection(int, void* );
int lowerh_color2,lowers_color2,lowerv_color2,upperh_color2,uppers_color2,upperv_color2;
char* lowerHc3;
char* lowerSc3;
char* lowerVc3;
char* upperHc3;
char* upperSc3;
char* upperVc3;
void color3_detection(int, void* );
int lowerh_color3,lowers_color3,lowerv_color3,upperh_color3,uppers_color3,upperv_color3;
char* lowerHc4;
char* lowerSc4;
char* lowerVc4;
char* upperHc4;
char* upperSc4;
char* upperVc4;
void color4_detection(int, void* );
int lowerh_color4,lowers_color4,lowerv_color4,upperh_color4,uppers_color4,upperv_color4;






int center_bot_frontx,center_bot_fronty,center_bot_backx,center_bot_backy,radius_front,radius_back;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);


int color1_cubes[2][2],color2_cubes[2][2],color3_cubes[2][2],color4_cubes[2][2];
int color1_cubes_detection();
int color2_cubes_detection();
int color3_cubes_detection();
int color4_cubes_detection();

int bot_front_color();
int bot_back_color();


int num = 0;

int main()
{
    VideoCapture cap(0);
    if(!cap.isOpened()) return -1;

    cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    bool running = true;


        while(running){



        if(num == 0){
        std::cout << "**************************Polaroid version 2********************************" << std::endl;
        std::cout << "Enter particular numbers to perform specific operations" << std::endl;
        std::cout << "1-To take screenshot for calibration" << std::endl;
        std::cout << "2-To crop arena"<< std:: endl;
        std::cout << "15-To crop island"<< std:: endl;
        std::cout << "3-To track pattern centers"<< std:: endl;
        std::cout << "4-To mark pattern center points"<<std::endl;
        std::cout << "5-To view pattern center points on original arena"<<std::endl;
        std::cout << "6-To enter pattern to be created 1-RED/2-BLUE/3-YELLOW"<<std::endl;
        std::cout << "7-To calibrate bot front color"<<std::endl;
        std::cout << "70-To verify bot front color"<<std::endl;
        std::cout << "8-To calibrate bot back color"<<std::endl;
        std::cout << "80-To verify bot back color"<<std::endl;
        std::cout << "11,21,31,41-color1,color2,color3,color4 cube detection"<<std::endl;
        std::cout << "12,22,32,42-color1,color2,color3,color4 cube verification"<<std::endl;
        std::cout << "10-start execution of program"<<std::endl;
        std::cin >> num ;
        }

        if(num==1){
            namedWindow("Original",1);
            cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
            cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);

             cap >> original;
            //original = imread( "best_1.jpg" );
            imshow("Original", original);

        }
        else if(num==2){
            namedWindow("Trackbars",CV_GUI_EXPANDED);
            createTrackbar( "Top","Trackbars", &top,original.rows,crop);
            createTrackbar( "Bottom","Trackbars", &bottom,original.rows,crop );
            createTrackbar( "Left","Trackbars", &left,original.cols,crop);
            createTrackbar( "Right","Trackbars", &right,original.cols,crop);



        }
        else if(num==15){
            namedWindow("Trackbars",CV_GUI_EXPANDED);
            createTrackbar( "Top","Trackbars", &top_is,original1.rows,crop_island);
            createTrackbar( "Bottom","Trackbars", &bottom_is,original1.rows,crop_island );
            createTrackbar( "Left","Trackbars", &left_is,original1.cols,crop_island);
            createTrackbar( "Right","Trackbars", &right_is,original1.cols,crop_island);

        }
        else if(num==3){
            namedWindow("Trackbars",CV_GUI_EXPANDED);
            createTrackbar( "Top","Trackbars", &ptop,original1.rows,crop_pattern);
            createTrackbar( "Bottom","Trackbars", &pbottom,original1.rows,crop_pattern);
            createTrackbar( "Left","Trackbars", &pleft,original1.cols,crop_pattern);
            createTrackbar( "Right","Trackbars", &pright,original1.cols,crop_pattern);



        }
         else if(num==4){
            int hdiv_factor = (pright-pleft)/3;
            int vdiv_factor = (pbottom-ptop)/3;

            for(int i = 0; i<3;i++){
                 pattern_points[i][0]= (hdiv_factor/2);
                 pattern_points[i][1]= (vdiv_factor/2)+ (i)*vdiv_factor;

            }
            for(int i = 3; i<6;i++){
                pattern_points[i][0]= (hdiv_factor/2)+hdiv_factor;
                pattern_points[i][1]= (vdiv_factor/2)+ (i-3)*vdiv_factor;
            }
            for(int i = 6; i<9;i++){
                pattern_points[i][0]= (hdiv_factor/2) + (2*hdiv_factor);
                pattern_points[i][1]= (vdiv_factor/2)+ (i-6)*vdiv_factor;
            }

            for(int i =0;i< 9;i++)
            {

                Point mark_center(pattern_points[i][0],pattern_points[i][1]);
                circle(pattern,mark_center, 3, Scalar(0,255,0), -1, 8, 0 );
            }
            imshow("NewArena",pattern);



        }
        else if(num==5){

            for(int i =0;i< 9;i++)
            {
                int add_factor =  original1.cols - pattern.cols;
                std::cout << add_factor << std::endl;
                //int new_x_cord = (add_factor+pattern_points[i][0]);
                pattern_points[i][0]= pattern_points[i][0];
                // pattern_points[i][1]= pattern_points[i][1]+ (hdiv_factor);
                Point mark_center(pattern_points[i][0],pattern_points[i][1]);
                circle(original1,mark_center, 3, Scalar(0,255,0), -1, 8, 0 );
            }
            imshow("NewArena",original1);



        }
        else if(num==6){


            for(int i =0;i< 9;i++)
            {

                std::cout << "Enter " << i << " colour"<< std::endl;
                std::cin >> pattern_color[i];

            }




        }
         else if(num==7){


           namedWindow("Trackbars",CV_GUI_EXPANDED);
            createTrackbar( "lowerHbf","Trackbars", &lowerh_bot_front,255,Threshold_bot_front);
            createTrackbar( "LowerSbf","Trackbars", &lowers_bot_front,255, Threshold_bot_front );
            createTrackbar( "LowerVbf","Trackbars", &lowerv_bot_front,255, Threshold_bot_front );
            createTrackbar( "upperHbf","Trackbars", &upperh_bot_front,255,Threshold_bot_front);
            createTrackbar( "upperSbf","Trackbars", &uppers_bot_front,255, Threshold_bot_front );
            createTrackbar( "upperVbf","Trackbars", &upperv_bot_front,255, Threshold_bot_front );




        }
        else if(num==70){

             bot_front_color();




        }
         else if(num==8){


           namedWindow("Trackbars",CV_GUI_EXPANDED);
            createTrackbar( "lowerHbb","Trackbars", &lowerh_bot_back,255,Threshold_bot_back);
            createTrackbar( "LowerSbb","Trackbars", &lowers_bot_back,255, Threshold_bot_back );
            createTrackbar( "LowerVbb","Trackbars", &lowerv_bot_back,255, Threshold_bot_back );
            createTrackbar( "upperHbb","Trackbars", &upperh_bot_back,255,Threshold_bot_back);
            createTrackbar( "upperSbb","Trackbars", &uppers_bot_back,255, Threshold_bot_back );
            createTrackbar( "upperVbb","Trackbars", &upperv_bot_back,255, Threshold_bot_back );




        }
        else if(num==80){

           bot_back_color();




        }
        else if(num==11){


           namedWindow("Trackbars",CV_GUI_EXPANDED);
            createTrackbar( "lowerHc1","Trackbars", &lowerh_color1,255,color1_detection);
            createTrackbar( "LowerSc1","Trackbars", &lowers_color1,255, color1_detection);
            createTrackbar( "LowerVc1","Trackbars", &lowerv_color1,255, color1_detection );
            createTrackbar( "upperHc1","Trackbars", &upperh_color1,255,color1_detection);
            createTrackbar( "upperSc1","Trackbars", &uppers_color1,255, color1_detection);
            createTrackbar( "upperVc1","Trackbars", &upperv_color1,255, color1_detection );




        }
        else if(num==12){

         color1_cubes_detection();

        }
        else if(num==21){


           namedWindow("Trackbars",CV_GUI_EXPANDED);
            createTrackbar( "lowerHc2","Trackbars", &lowerh_color2,255,color2_detection);
            createTrackbar( "LowerSc2","Trackbars", &lowers_color2,255, color2_detection);
            createTrackbar( "LowerVc2","Trackbars", &lowerv_color2,255, color2_detection );
            createTrackbar( "upperHc2","Trackbars", &upperh_color2,255,color2_detection);
            createTrackbar( "upperSc2","Trackbars", &uppers_color2,255, color2_detection);
            createTrackbar( "upperVc2","Trackbars", &upperv_color2,255, color2_detection );




        }
        else if(num==22){


              color2_cubes_detection();
        }
        else if(num==31){


           namedWindow("Trackbars",CV_GUI_EXPANDED);
            createTrackbar( "lowerHc3","Trackbars", &lowerh_color3,255,color3_detection);
            createTrackbar( "LowerSc3","Trackbars", &lowers_color3,255, color3_detection);
            createTrackbar( "LowerVc3","Trackbars", &lowerv_color3,255, color3_detection );
            createTrackbar( "upperHc3","Trackbars", &upperh_color3,255,color3_detection);
            createTrackbar( "upperSc3","Trackbars", &uppers_color3,255, color3_detection);
            createTrackbar( "upperVc3","Trackbars", &upperv_color3,255, color3_detection );




        }
        else if(num==32){

             color3_cubes_detection();

        }
        else if(num == 41){
            namedWindow("Trackbars",CV_GUI_EXPANDED);
            createTrackbar( "lowerHc4","Trackbars", &lowerh_color4,255,color4_detection);
            createTrackbar( "LowerSc4","Trackbars", &lowers_color4,255, color4_detection);
            createTrackbar( "LowerVc4","Trackbars", &lowerv_color4,255, color4_detection );
            createTrackbar( "upperHc4","Trackbars", &upperh_color4,255,color4_detection);
            createTrackbar( "upperSc4","Trackbars", &uppers_color4,255, color4_detection);
            createTrackbar( "upperVc4","Trackbars", &upperv_color4,255, color4_detection );
        }
        else if(num == 42){
            color4_cubes_detection();

        }
        else if(num==10){
             HANDLE serial = openSerialPort(PORTNAME,B9600,one,off);
             for(int i= 0; i<9;i++){
                if(pattern_color[i]==0){
                    continue;
                }
                else{

                int shortest_dist;
                int follow_pt[1][1];
                int followed = 0;
                int horizontally_oriented = 0;
                int inline_with_pattern = 0;
                int vertically_oriented = 0;
                int reached_destination = 0;


                for(;;){
                namedWindow("Original",1);
                cap.set(CV_CAP_PROP_FRAME_WIDTH,640);
                cap.set(CV_CAP_PROP_FRAME_HEIGHT,480);
                cap >> original;
                Rect rect,rect1;
                rect = Rect(left,top,right-left,bottom-top);
                original1 = original(rect);

                rect1 = Rect(left_is,top_is,right_is-left_is,bottom_is-top_is);
                island = original1(rect1);
                //original = imread( "interceptor_final_arena2.jpg" );
                imshow("Original", original1);

                int size1,size2,size3;
                //size1 = color1_cubes_detection();
                //size2 = color2_cubes_detection();
                //size3 = color3_cubes_detection();
                //if(size1>2 && size2>2 && size3>2){
                  //  continue;
                //}
               // int cubes_array[6][2] = {0};

               // cubes_array[6][2] = color1_cubes[2][2] + color2_cubes[2][2] + color3_cubes[2][2];

                //int frontsize = bot_front_color();
                //int backsize = bot_back_color();

                //if(frontsize!=1 && backsize != 1){
                  //  continue;
                //}
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        //cvtColor(island,arena_calib, CV_BGR2GRAY );

        color1_cubes[2][2] = {0};
        inRange(island,Scalar(lowerh_color1,lowers_color1,lowerv_color1),Scalar(upperh_color1,uppers_color1,upperv_color1),arena_calib2);
        GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
        threshold(arena_calib2,block_arena, thresh, 255, THRESH_BINARY );
        findContours(arena_calib2, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

          /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );


        Mat drawing = Mat::zeros(island.size(), CV_8UC3 );
        for( size_t iw = 0; iw < contours.size(); iw++ )
        {   approxPolyDP( Mat(contours[iw]), contours_poly[iw], 3, true );
            boundRect[iw] = boundingRect( Mat(contours_poly[iw]) );
            float f = 5.00;
            minEnclosingCircle( contours_poly[iw], center[iw],*&f);
            std::cout << center[iw].x << std::endl;
            std::cout << center[iw].y + top_is<< std::endl;
            color1_cubes[iw][0] = center[iw].x ;
            color1_cubes[iw][1] =  center[iw].y + top_is;

        }


        /// Draw polygonal contour + bonding rects + circles

        for( size_t iw = 0; iw< contours.size(); iw++ )
       {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(drawing, contours_poly, (int)iw, color, 1, 8, vector<Vec4i>(), 0, Point() );
            rectangle( drawing, boundRect[iw].tl(), boundRect[iw].br(), color, 2, 8, 0 );
            circle( drawing, center[iw], (int)radius[iw], color, 2, 8, 0 );
       }


            namedWindow( "Contours", WINDOW_AUTOSIZE );


        vector<vector<Point> > contours1;
        vector<Vec4i> hierarchy1;

        //cvtColor(island,arena_calib, CV_BGR2GRAY );


        inRange(island,Scalar(lowerh_color2,lowers_color2,lowerv_color2),Scalar(upperh_color2,uppers_color2,upperv_color2),arena_calib2);
        GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
        threshold(arena_calib2,block_arena, thresh, 255, THRESH_BINARY );
        findContours(arena_calib2, contours1, hierarchy1,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

          /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly1( contours1.size() );
        vector<Rect> boundRect1( contours1.size() );
        vector<Point2f>center1( contours1.size() );
        vector<float>radius1( contours1.size() );
       // Mat drawing = Mat::zeros( original1.size(), CV_8UC3 );
        color2_cubes[2][2] = {0};
        for( size_t iw = 0; iw < contours1.size(); iw++ )
        {   approxPolyDP( Mat(contours1[iw]), contours_poly1[iw], 3, true );
            boundRect1[iw] = boundingRect( Mat(contours_poly1[iw]) );
            float f = 5.00;
            minEnclosingCircle( contours_poly1[iw], center1[iw],*&f);
            std::cout << center1[iw].x << std::endl;
            std::cout << center1[iw].y + top_is<< std::endl;
            color2_cubes[iw][0] = center1[iw].x;
            color2_cubes[iw][1] =  center1[iw].y + top_is;
       //*&botspx = center[i].x;
       //*&botspy = center[i].y;

        }


        /// Draw polygonal contour + bonding rects + circles

        for( size_t iw = 0; iw< contours1.size(); iw++ )
       {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(drawing, contours_poly1, (int)iw, color, 1, 8, vector<Vec4i>(), 0, Point() );
            rectangle( drawing, boundRect1[iw].tl(), boundRect1[iw].br(), color, 2, 8, 0 );
            circle( drawing, center1[iw], (int)radius1[iw], color, 2, 8, 0 );
       }


        vector<vector<Point> > contours2;
        vector<Vec4i> hierarchy2;

        //cvtColor(island,arena_calib, CV_BGR2GRAY );


        inRange(island,Scalar(lowerh_color3,lowers_color3,lowerv_color3),Scalar(upperh_color3,uppers_color3,upperv_color3),arena_calib2);
        GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
        threshold(arena_calib2,block_arena, thresh, 255, THRESH_BINARY );
        findContours(arena_calib2, contours2, hierarchy2,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

          /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly2( contours2.size() );
        vector<Rect> boundRect2( contours2.size() );
        vector<Point2f>center2( contours2.size() );
        vector<float>radius2( contours2.size() );

        color3_cubes[2][2] = {0};
        //Mat drawing = Mat::zeros( original1.size(), CV_8UC3 );
        for( size_t iw = 0; iw < contours2.size(); iw++ )
        {   approxPolyDP( Mat(contours2[iw]), contours_poly2[iw], 3, true );
            boundRect2[iw] = boundingRect( Mat(contours_poly2[iw]) );
            float f = 5.00;
            minEnclosingCircle( contours_poly2[iw], center2[iw],*&f);
            std::cout << center2[iw].x << std::endl;
            std::cout << center2[iw].y + top_is<< std::endl;
            color3_cubes[iw][0] = center2[iw].x;
            color3_cubes[iw][1] =  center2[iw].y+ top_is;
       //*&botspx = center[i].x;
       //*&botspy = center[i].y;

        }


        /// Draw polygonal contour + bonding rects + circles

        for( size_t iw = 0; iw< contours2.size(); iw++ )
       {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(drawing, contours_poly2, (int)iw, color, 1, 8, vector<Vec4i>(), 0, Point() );
            rectangle( drawing, boundRect2[iw].tl(), boundRect2[iw].br(), color, 2, 8, 0 );
            circle( drawing, center2[iw], (int)radius2[iw], color, 2, 8, 0 );
       }



        vector<vector<Point> > contours3;
        vector<Vec4i> hierarchy3;

        inRange(island,Scalar(lowerh_color4,lowers_color4,lowerv_color4),Scalar(upperh_color4,uppers_color4,upperv_color4),arena_calib2);
        GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
        threshold(arena_calib2,block_arena, thresh, 255, THRESH_BINARY );
        findContours(arena_calib2, contours3, hierarchy2,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

          /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly3( contours2.size() );
        vector<Rect> boundRect3( contours3.size() );
        vector<Point2f>center3( contours3.size() );
        vector<float>radius3( contours3.size() );

        color4_cubes[2][2] = {0};
        //Mat drawing = Mat::zeros( original1.size(), CV_8UC3 );
        for( size_t iw = 0; iw < contours3.size(); iw++ )
        {   approxPolyDP( Mat(contours3[iw]), contours_poly3[iw], 3, true );
            boundRect3[iw] = boundingRect( Mat(contours_poly3[iw]) );
            float f = 5.00;
            minEnclosingCircle( contours_poly3[iw], center3[iw],*&f);
            std::cout << center3[iw].x << std::endl;
            std::cout << center3[iw].y + top_is<< std::endl;
            color4_cubes[iw][0] = center3[iw].x;
            color4_cubes[iw][1] =  center3[iw].y+ top_is;
       //*&botspx = center[i].x;
       //*&botspy = center[i].y;

        }


        /// Draw polygonal contour + bonding rects + circles

       for( size_t iw = 0; iw< contours3.size(); iw++ )
       {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(drawing, contours_poly2, (int)iw, color, 1, 8, vector<Vec4i>(), 0, Point() );
            rectangle( drawing, boundRect2[iw].tl(), boundRect2[iw].br(), color, 2, 8, 0 );
            circle( drawing, center3[iw], (int)radius3[iw], color, 2, 8, 0 );
       }


         //cvtColor(original1,arena_calib, CV_BGR2GRAY );

          inRange(original1,Scalar(lowerh_bot_front,lowers_bot_front,lowerv_bot_front),Scalar(upperh_bot_front,uppers_bot_front,upperv_bot_front),arena_calib2);
          GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
          vector<Vec3f> circles;
           HoughCircles(arena_calib2, circles, CV_HOUGH_GRADIENT, 1, arena_calib2.rows/16, 200,100, 0, 60);
           for( size_t iw = 0; iw < circles.size(); iw++ )
            {
                Point center(cvRound(circles[iw][0]), cvRound(circles[iw][1]));
               // int radius = cvRound(circles[i][2]);
                std::cout << cvRound(circles[iw][0])<< cvRound(circles[iw][1]) <<std::endl;
               *&center_bot_frontx = cvRound(circles[iw][0]);
               *&center_bot_fronty = cvRound(circles[iw][1]);
               *&radius_front = cvRound(circles[iw][2]);
                // circle center
                //circle(original1, center, 3, Scalar(0,255,0), -1, 8, 0 );
                // circle outline
                //circle(original1, center, radius, Scalar(0,45,255), 3, 8, 0 );
            }



          //cvtColor(original1,arena_calib, CV_BGR2GRAY );

          inRange(original1,Scalar(lowerh_bot_back,lowers_bot_back,lowerv_bot_back),Scalar(upperh_bot_back,uppers_bot_back,upperv_bot_back),arena_calib2);
          GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
          vector<Vec3f> circles1;
           HoughCircles(arena_calib2, circles1, CV_HOUGH_GRADIENT, 1, arena_calib.rows/16, 200,100, 0, 60);
           for( size_t iw = 0; iw < circles1.size(); iw++ )
            {
                Point center(cvRound(circles1[iw][0]), cvRound(circles1[iw][1]));
               // int radius = cvRound(circles1[i][2]);
                std::cout << cvRound(circles1[iw][0])<< cvRound(circles1[iw][1]) <<std::endl;
                *&center_bot_backx = cvRound(circles1[iw][0]);
                *&center_bot_backy = cvRound(circles1[iw][1]);
                *&radius_back =  cvRound(circles[iw][2]);

                // circle center
                //circle(original1, center, 3, Scalar(0,255,0), -1, 8, 0 );
                // circle outline
                //circle(original1, center, radius, Scalar(0,45,255), 3, 8, 0 );
            }




        /// Show in a window
            namedWindow( "Contours", WINDOW_AUTOSIZE );
            imshow( "Contours",drawing );




            Point center1pt(center_bot_frontx,center_bot_fronty);
            circle(original1,center1pt, 3, Scalar(0,255,0), -1, 8, 0 );    //plot bot front center
            circle( original1,center1pt ,radius_front, Scalar(0,255,0), 3, 8, 0 );

            Point center2pt(center_bot_backx, center_bot_backy);
            circle( original1,center2pt, 3, Scalar(0,255,0), -1, 8, 0 );    //plot bot back center
            circle( original1,center2pt ,radius_back, Scalar(0,00,255), 3, 8, 0 );

            //namedWindow( "Original", WINDOW_AUTOSIZE );
            imshow("Original", original1);


















                    double distbx = center_bot_backx-center_bot_frontx;              //calculates bot angle
                    double distby = center_bot_backy-center_bot_fronty;
                    double angleb,anglebd;
                    if(distbx<0 && distby >0){
                    distbx = distbx*-1;
                    angleb = atan(distby/distbx);
                    anglebd = angleb*180/3.14;
                    }
                    else if(distbx>0 && distby >0){
                    angleb = atan(distby/distbx);
                    anglebd = 180 -(angleb*180/3.14);
                    }
                    else if(distbx>0 && distby <0){
                    distby = distby*-1;
                    angleb = atan(distby/distbx);
                    anglebd = 180+angleb*180/3.14;

                    }
                    else if(distbx<0 && distby <0){
                    distby = distby*-1;
                    distbx = distbx*-1;
                    angleb = atan(distby/distbx);
                    anglebd = 360 -(angleb*180/3.14);
                    }
                        std::cout <<"Orientation of bot "<<anglebd << std::endl;
                    if(followed==0){


                    if(pattern_color[i]==1){
                    if(color1_cubes[1][0] == 0){
                        int temp = pow((center_bot_frontx - color1_cubes[0][0]),2)+ ((center_bot_fronty - color1_cubes[0][1]),2);
                        shortest_dist = sqrt(temp);
                        follow_pt[0][0]= color1_cubes[0][0];
                        follow_pt[0][1]= color1_cubes[0][1];
                    }
                    else{
                        int temp1 = pow((center_bot_frontx - color1_cubes[0][0]),2)+ ((center_bot_fronty - color1_cubes[0][1]),2);
                        int temp2 = pow((center_bot_frontx - color1_cubes[1][0]),2)+ ((center_bot_fronty - color1_cubes[1][1]),2);
                        if(temp1>=temp2){
                            shortest_dist = temp2;
                            follow_pt[0][0]= color2_cubes[1][0];
                            follow_pt[0][1]= color2_cubes[1][1];
                        }
                        else{
                            shortest_dist = temp1;
                            follow_pt[0][0]= color1_cubes[0][0];
                            follow_pt[0][1]= color1_cubes[0][1];
                        }
                    }

                }else if(pattern_color[i]==2){
                    if(color2_cubes[1][0] == 0){
                        int temp = pow((center_bot_frontx - color2_cubes[0][0]),2)+ ((center_bot_fronty - color2_cubes[0][1]),2);
                        shortest_dist = sqrt(temp);
                        follow_pt[0][0]= color2_cubes[0][0];
                        follow_pt[0][1]= color2_cubes[0][1];
                    }
                    else{
                        int temp1 = pow((center_bot_frontx - color2_cubes[0][0]),2)+ ((center_bot_fronty - color2_cubes[0][1]),2);
                        int temp2 = pow((center_bot_frontx - color2_cubes[1][0]),2)+ ((center_bot_fronty - color2_cubes[1][1]),2);
                        if(temp1>=temp2){
                            shortest_dist = temp2;
                            follow_pt[0][0]= color2_cubes[1][0];
                            follow_pt[0][1]= color2_cubes[1][1];
                        }
                        else{
                            shortest_dist = temp1;
                            follow_pt[0][0]= color2_cubes[0][0];
                            follow_pt[0][1]= color2_cubes[0][1];
                        }
                    }

                }else if(pattern_color[i]==3){
                    if(color3_cubes[1][0] == 0){
                        int temp = pow((center_bot_frontx - color3_cubes[0][0]),2)+ ((center_bot_fronty - color3_cubes[0][1]),2);
                        shortest_dist = sqrt(temp);
                        follow_pt[0][0]= color3_cubes[0][0];
                        follow_pt[0][1]= color3_cubes[0][1];
                    }
                    else{
                        int temp1 = pow((center_bot_frontx - color3_cubes[0][0]),2)+ ((center_bot_fronty - color3_cubes[0][1]),2);
                        int temp2 = pow((center_bot_frontx - color3_cubes[1][0]),2)+ ((center_bot_fronty - color3_cubes[1][1]),2);
                        if(temp1>=temp2){
                            shortest_dist = temp2;
                            follow_pt[0][0]= color3_cubes[1][0];
                            follow_pt[0][1]= color3_cubes[1][1];
                        }
                        else{
                            shortest_dist = temp1;
                            follow_pt[0][0]= color3_cubes[0][0];
                            follow_pt[0][1]= color3_cubes[0][1];
                        }
                    }
                    }else if(pattern_color[i]==4){
                    if(color4_cubes[1][0] == 0){
                        int temp = pow((center_bot_frontx - color4_cubes[0][0]),2)+ ((center_bot_fronty - color4_cubes[0][1]),2);
                        shortest_dist = sqrt(temp);
                        follow_pt[0][0]= color4_cubes[0][0];
                        follow_pt[0][1]= color4_cubes[0][1];
                    }else{
                        int temp1 = pow((center_bot_frontx - color4_cubes[0][0]),2)+ ((center_bot_fronty - color4_cubes[0][1]),2);
                        int temp2 = pow((center_bot_frontx - color4_cubes[1][0]),2)+ ((center_bot_fronty - color4_cubes[1][1]),2);
                        if(temp1>=temp2){
                            shortest_dist = temp2;
                            follow_pt[0][0]= color4_cubes[1][0];
                            follow_pt[0][1]= color4_cubes[1][1];
                        }else{
                            shortest_dist = temp1;
                            follow_pt[0][0]= color4_cubes[0][0];
                            follow_pt[0][1]= color4_cubes[0][1];
                        }
                    }

                }


                    double distox = center_bot_backx-follow_pt[0][0];              //calculates object angle
                    double distoy = center_bot_backy-follow_pt[0][1];
                    double angleo,angleod;
                    if(distox<0 && distoy >0){
                    distox = distox*-1;
                    angleo = atan(distoy/distox);
                    angleod = angleo*180/3.14;
                    }
                    else if(distox>0 && distoy >0){
                    angleo = atan(distoy/distox);
                    angleod = 180 -(angleo*180/3.14);
                    }
                    else if(distox>0 && distoy <0){
                    distoy = distoy*-1;
                    angleo = atan(distoy/distox);
                    angleod = 180+angleo*180/3.14;

                    }
                    else if(distox<0 && distoy <0){
                    distoy = distoy*-1;
                    distox = distox*-1;
                    angleo = atan(distoy/distox);
                    angleod = 360 -(angleo*180/3.14);
                    }
                        std::cout <<"Orientation of object "<<angleod << std::endl;


                    if(angleod>=anglebd){
                            if(anglebd<90 && angleod>270){
                                std::cout<<"Turn right"<< std::endl;
                                writeToSerialPort(serial,RIGHT,1);
                                Sleep(150);
                                writeToSerialPort(serial,STOP,1);
                            }
                            else if(angleod-anglebd<5){
                                int distx = (center_bot_frontx-follow_pt[0][0])*(center_bot_frontx-follow_pt[0][0]);
                                int disty = (center_bot_fronty-follow_pt[0][1])* (center_bot_fronty-follow_pt[0][1]);
                                double dist = sqrt(distx+disty);
                                if(dist<40){
                                    std::cout<<"Stop"<< std::endl;
                                    writeToSerialPort(serial,STOP,1);
                                    Sleep(150);
                                }
                                else{
                                    std::cout<<"Move forward"<<std::endl;
                                    writeToSerialPort(serial,FRONT,1);
                                    Sleep(150);
                                    writeToSerialPort(serial,STOP,1);
                                }

                        }
                        else{
                        std::cout << "turn left" << std::endl;
                        writeToSerialPort(serial,LEFT,1);
                        Sleep(150);
                        writeToSerialPort(serial,STOP,1);

                        }
                    }
                    else{
                            if(angleod<90 && anglebd>270){
                                std::cout<<"Turn left"<< std::endl;
                                writeToSerialPort(serial,LEFT,1);
                                Sleep(150);
                                writeToSerialPort(serial,STOP,1);
                            }
                            else if(anglebd-angleod<15){
                                int distx = (center_bot_frontx-follow_pt[0][0])*(center_bot_frontx-follow_pt[0][0]);
                                int disty = (center_bot_fronty-follow_pt[0][1])* (center_bot_fronty-follow_pt[0][1]);
                                double dist = sqrt(distx+disty);
                                if(dist<30){
                                    std::cout<<"Stop"<< std::endl;
                                    writeToSerialPort(serial,STOP,1);
                                    Sleep(200);
                                    std::cout << "Pick the cube" << std::endl;
                                    writeToSerialPort(serial,DOWNL,1);
                                    Sleep(300);
                                    writeToSerialPort(serial,STOPL,1);

                                    followed = 1;
                                }
                                else{
                                    std::cout<<"Move forward"<<std::endl;
                                    writeToSerialPort(serial,FRONT,1);
                                    Sleep(150);
                                    writeToSerialPort(serial,STOP,1);
                                }

                            }
                            else{
                             std::cout << "Turn right" << std::endl;
                             writeToSerialPort(serial,RIGHT,1);
                             Sleep(150);
                             writeToSerialPort(serial,STOP,1);

                            }
                    }
                    }else if(followed == 1 && horizontally_oriented == 0){

                    if(anglebd>0 && anglebd<180){
                        if(anglebd<10){
                            std::cout << "Stop" <<std::endl;
                            writeToSerialPort(serial,STOP,1);
                            Sleep(150);
                            horizontally_oriented = 1;
                        }
                            std::cout <<"Turn right" <<std::endl;
                            writeToSerialPort(serial,RIGHT,1);
                            Sleep(150);
                            writeToSerialPort(serial,STOP,1);
                    }
                    else{
                        if(anglebd>350){
                            std::cout << "Stop" << std::endl;
                            horizontally_oriented = 1;
                            writeToSerialPort(serial,STOP,1);
                            Sleep(150);
                        }
                        std::cout << "Turn left" << std::endl;
                        writeToSerialPort(serial,LEFT,1);
                        Sleep(150);
                        writeToSerialPort(serial,STOP,1);
                    }


                    }else if(horizontally_oriented==1 && inline_with_pattern == 0){
                           if(pattern_points[i][0]<center_bot_backx){
                           if((pattern_points[i][0]-center_bot_backx)<20){
                            std::cout << "Stop" << std::endl;
                            writeToSerialPort(serial,STOP,1);
                            Sleep(150);
                            inline_with_pattern = 1;
                           }
                             std::cout << "move back" <<std::endl;
                             writeToSerialPort(serial,BACK,1);
                             Sleep(200);
                             writeToSerialPort(serial,STOP,1);
                           }
                           else{
                               std::cout << "Stop" << std::endl;
                                writeToSerialPort(serial,STOP,1);
                                Sleep(150);
                                inline_with_pattern = 1;
                           }
                    }
                    else if(inline_with_pattern == 1 && vertically_oriented ==0){
                        if(anglebd<90){
                            if(anglebd>80){
                                std::cout << "stop" << std::endl;
                                Sleep(150);
                                vertically_oriented = 1;
                                writeToSerialPort(serial,STOP,1);
                            }
                            std::cout <<"Turn left" << std:: endl;
                            writeToSerialPort(serial,LEFT,1);
                            Sleep(150);
                            writeToSerialPort(serial,STOP,1);
                        }
                        else{
                            if(anglebd < 100){
                                std::cout << "Stop" << std::endl;
                                Sleep(150);
                                vertically_oriented = 1;
                                writeToSerialPort(serial,STOP,1);

                            }
                            std::cout << "Turn Right" << std::endl;

                            writeToSerialPort(serial,RIGHT,1);
                            Sleep(150);
                            writeToSerialPort(serial,STOP,1);


                        }


                    }
                    else if(vertically_oriented == 1 && reached_destination == 0 ){
                        if(center_bot_fronty>pattern_points[i][1]){
                            if((pattern_points[i][1]-center_bot_fronty)<30)
                            {
                                std::cout << "Stop , Drop the cube" << std::endl;

                                reached_destination = 1;
                                writeToSerialPort(serial,STOP,1);
                                Sleep(150);



                                writeToSerialPort(serial,UPL,1);
                                Sleep(300);
                                writeToSerialPort(serial,STOPL,1);
                            }
                            std::cout << "Move front" << std::endl;

                            writeToSerialPort(serial,STOP,1);
                            Sleep(150);
                        }


                    }else if (reached_destination == 1){
                             if(center_bot_backy < 80){
                                if(center_bot_backy>40){
                                    std::cout << "Stop" <<std::endl;
                                    writeToSerialPort(serial,STOP,1);
                                    Sleep(150);
                                    followed = 0;
                                    horizontally_oriented = 0;
                                    inline_with_pattern = 0;
                                    vertically_oriented = 0;
                                    reached_destination = 0;

                                    break;
                                }
                                std::cout  << "move back" << std::endl;
                                writeToSerialPort(serial,BACK,1);
                                Sleep(150);
                                writeToSerialPort(serial,STOP,1);
                             }
                             else{
                                std::cout  << "move stop" << std::endl;


                                writeToSerialPort(serial,STOP,1);
                                Sleep(150);
                             }

                    }








                          system("CLS");
                          if(waitKey(30) >= 0)     break;


                    }
                }



                }
             }
        //}





        //
        system("CLS");
         num = 0;


//}

    waitKey( 1200000 );
    destroyAllWindows();
    }




    return 0;
}
void crop( int,void*)
{
            Rect rect;
            rect = Rect(left,top,right-left,bottom-top);
            *&left = left;
            *&top = top;
            *&right = right;
            *&bottom = bottom;

            original1 = original(rect);

            pright = original1.cols;
            pbottom = original1.rows;

            right_is = original1.cols;
            bottom_is = original1.rows;
            cvtColor(original1,arena_calib, CV_BGR2GRAY );


            imshow("NewArena",original1);
}
void crop_pattern( int,void*)
{
            Rect rect;
            rect = Rect(pleft,ptop,pright-pleft,pbottom-ptop);
            *&pleft = pleft;
            *&ptop = ptop;
            *&pright = pright;
            *&pbottom = pbottom;



            pattern = original1(rect);
            imshow("NewArena",pattern);
}
void crop_island( int,void*)
{
            Rect rect;
            rect = Rect(left_is,top_is,right_is-left_is,bottom_is-top_is);
            *&left_is = left_is;
            *&top_is = top_is;
            *&right_is = right_is;
            *&bottom_is = bottom_is;



            island = original1(rect);
            imshow("NewArena",island);
}
void rotate_fn( int,void*)
{
            //double angle = 180;  // or 270
            Size src_sz = original.size();
            Size dst_sz(src_sz.height, src_sz.width);

            int len = std::max(original.cols,original.rows);
            Point2f center(len/2., len/2.);
            Mat rot_mat = cv::getRotationMatrix2D(center, angle, 1.0);
            warpAffine(original,original1, rot_mat, dst_sz);
            imshow("Rotated arena",original1);
}
void Threshold_bot_front( int,void*)
{
    namedWindow("NewArena",1);


     //cvtColor(original1,arena_calib, CV_BGR2GRAY );
    inRange(original1,Scalar(lowerh_bot_front,lowers_bot_front,lowerv_bot_front),Scalar(upperh_bot_front,uppers_bot_front,upperv_bot_front),arena_calib2);
     GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
     //Canny(arena_calib2,arena_calib2, 0, 30, 3);
    *&lowerh_bot_front = lowerh_bot_front;
    *&lowers_bot_front = lowers_bot_front;
    *&lowerv_bot_front = lowerv_bot_front;
    *&upperh_bot_front = upperh_bot_front;
    *&uppers_bot_front = uppers_bot_front;
    *&upperv_bot_front = upperv_bot_front;
    imshow( "NewArena",arena_calib2 );
}
void Threshold_bot_back( int,void*)
{
    namedWindow("NewArena",1);
   //  cvtColor(original1,arena_calib, CV_BGR2GRAY );


    inRange(original1,Scalar(lowerh_bot_back,lowers_bot_back,lowerv_bot_back),Scalar(upperh_bot_back,uppers_bot_back,upperv_bot_back),arena_calib2);
    GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
    *&lowerh_bot_back = lowerh_bot_back;
    *&lowers_bot_back = lowers_bot_back;
    *&lowerv_bot_back = lowerv_bot_back;
    *&upperh_bot_back = upperh_bot_back;
    *&uppers_bot_back = uppers_bot_back;
    *&upperv_bot_back = upperv_bot_back;
    imshow( "NewArena",arena_calib2 );
}


void color1_detection(int, void* )
{


  /// Detect edges using Threshold
   //cvtColor(original1,arena_calib, CV_BGR2GRAY );

   inRange(original1,Scalar(lowerh_color1,lowers_color1,lowerv_color1),Scalar(upperh_color1,uppers_color1,upperv_color1),arena_calib2);
   GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
   threshold(arena_calib,block_arena, thresh, 255, THRESH_BINARY );
   imshow( "Colour 1 calibration",arena_calib2 );

}
void color2_detection(int, void* )
{


  /// Detect edges using Threshold
   //cvtColor(original1,arena_calib, CV_BGR2GRAY );

   inRange(original1,Scalar(lowerh_color2,lowers_color2,lowerv_color2),Scalar(upperh_color2,uppers_color2,upperv_color2),arena_calib2);
   GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
   threshold(arena_calib,block_arena, thresh, 255, THRESH_BINARY );
   imshow( "Colour 2 calibration",arena_calib2 );

}
void color3_detection(int, void* )
{


  /// Detect edges using Threshold
   //cvtColor(original1,arena_calib, CV_BGR2GRAY );

   inRange(original1,Scalar(lowerh_color3,lowers_color3,lowerv_color3),Scalar(upperh_color3,uppers_color3,upperv_color3),arena_calib2);
   GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
   threshold(arena_calib,block_arena, thresh, 255, THRESH_BINARY );
   imshow( "Colour 3 calibration",arena_calib2 );

}
void color4_detection(int, void* )
{


  /// Detect edges using Threshold
   //cvtColor(original1,arena_calib, CV_BGR2GRAY );

   inRange(original1,Scalar(lowerh_color4,lowers_color4,lowerv_color4),Scalar(upperh_color4,uppers_color4,upperv_color4),arena_calib2);
   GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
   threshold(arena_calib,block_arena, thresh, 255, THRESH_BINARY );
   imshow( "Colour 4 calibration",arena_calib2 );

}
int color1_cubes_detection(){

        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

      //  cvtColor(original1,arena_calib, CV_BGR2GRAY );


        inRange(original1,Scalar(lowerh_color1,lowers_color1,lowerv_color1),Scalar(upperh_color1,uppers_color1,upperv_color1),arena_calib2);
        GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
        threshold(arena_calib,block_arena, thresh, 255, THRESH_BINARY );
        findContours(arena_calib2, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

          /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );

        color1_cubes[2][2] = {0};
        Mat drawing = Mat::zeros( original1.size(), CV_8UC3 );
        for( size_t iw = 0; iw < contours.size(); iw++ )
        {   approxPolyDP( Mat(contours[iw]), contours_poly[iw], 3, true );
            boundRect[iw] = boundingRect( Mat(contours_poly[iw]) );
            float f = 5.00;
            minEnclosingCircle( contours_poly[iw], center[iw],*&f);
            std::cout << center[iw].x << std::endl;
            std::cout << center[iw].y << std::endl;
            color1_cubes[iw][0] = center[iw].x;
            color1_cubes[iw][1] =  center[iw].y;
       //*&botspx = center[i].x;
       //*&botspy = center[i].y;

        }


        /// Draw polygonal contour + bonding rects + circles

        for( size_t iw = 0; iw< contours.size(); iw++ )
       {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(drawing, contours_poly, (int)iw, color, 1, 8, vector<Vec4i>(), 0, Point() );
            rectangle( drawing, boundRect[iw].tl(), boundRect[iw].br(), color, 2, 8, 0 );
            circle( drawing, center[iw], (int)radius[iw], color, 2, 8, 0 );
       }

        /// Show in a window
            namedWindow( "Contours", WINDOW_AUTOSIZE );
            imshow( "Contours",drawing );

             return contours.size();

}
int color2_cubes_detection(){
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        //cvtColor(original1,arena_calib, CV_BGR2GRAY );


        inRange(original1,Scalar(lowerh_color2,lowers_color2,lowerv_color2),Scalar(upperh_color2,uppers_color2,upperv_color2),arena_calib2);
        GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
        threshold(arena_calib2,block_arena, thresh, 255, THRESH_BINARY );
        findContours(arena_calib2, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

          /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );
        Mat drawing = Mat::zeros( original1.size(), CV_8UC3 );
        color2_cubes[2][2] = {0};
        for( size_t i = 0; i < contours.size(); i++ )
        {   approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            float f = 5.00;
            minEnclosingCircle( contours_poly[i], center[i],*&f);
            std::cout << center[i].x << std::endl;
            std::cout << center[i].y << std::endl;
            color2_cubes[i][0] = center[i].x;
            color2_cubes[i][1] =  center[i].y;
       //*&botspx = center[i].x;
       //*&botspy = center[i].y;

        }


        /// Draw polygonal contour + bonding rects + circles

        for( size_t i = 0; i< contours.size(); i++ )
       {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
            rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
            circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       }

        /// Show in a window
            namedWindow( "Contours", WINDOW_AUTOSIZE );
            imshow( "Contours",drawing );

             return contours.size();
}
int color3_cubes_detection(){
vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        //cvtColor(original1,arena_calib, CV_BGR2GRAY );


        inRange(original1,Scalar(lowerh_color3,lowers_color3,lowerv_color3),Scalar(upperh_color3,uppers_color3,upperv_color3),arena_calib2);
        GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
        threshold(arena_calib,block_arena, thresh, 255, THRESH_BINARY );
        findContours(arena_calib2, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

          /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );

        color3_cubes[2][2] = {0};
        Mat drawing = Mat::zeros( original1.size(), CV_8UC3 );
        for( size_t i = 0; i < contours.size(); i++ )
        {   approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            float f = 5.00;
            minEnclosingCircle( contours_poly[i], center[i],*&f);
            std::cout << center[i].x << std::endl;
            std::cout << center[i].y << std::endl;
            color3_cubes[i][0] = center[i].x;
            color3_cubes[i][1] =  center[i].y;
       //*&botspx = center[i].x;
       //*&botspy = center[i].y;

        }


        /// Draw polygonal contour + bonding rects + circles

        for( size_t i = 0; i< contours.size(); i++ )
       {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
            rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
            circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       }



        /// Show in a window
            namedWindow( "Contours", WINDOW_AUTOSIZE );
            imshow( "Contours",drawing );

            return contours.size();
}
int color4_cubes_detection(){
vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;

        //cvtColor(original1,arena_calib, CV_BGR2GRAY );


        inRange(original1,Scalar(lowerh_color4,lowers_color4,lowerv_color4),Scalar(upperh_color4,uppers_color4,upperv_color4),arena_calib2);
        GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
        threshold(arena_calib,block_arena, thresh, 255, THRESH_BINARY );
        findContours(arena_calib2, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

          /// Approximate contours to polygons + get bounding rects and circles
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );

        color4_cubes[2][2] = {0};
        Mat drawing = Mat::zeros( original1.size(), CV_8UC3 );
        for( size_t i = 0; i < contours.size(); i++ )
        {   approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            float f = 5.00;
            minEnclosingCircle( contours_poly[i], center[i],*&f);
            std::cout << center[i].x << std::endl;
            std::cout << center[i].y << std::endl;
            color4_cubes[i][0] = center[i].x;
            color4_cubes[i][1] =  center[i].y;
       //*&botspx = center[i].x;
       //*&botspy = center[i].y;

        }


        /// Draw polygonal contour + bonding rects + circles

        for( size_t i = 0; i< contours.size(); i++ )
       {
            Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
            drawContours(drawing, contours_poly, (int)i, color, 1, 8, vector<Vec4i>(), 0, Point() );
            rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
            circle( drawing, center[i], (int)radius[i], color, 2, 8, 0 );
       }



        /// Show in a window
            namedWindow( "Contours", WINDOW_AUTOSIZE );
            imshow( "Contours",drawing );

            return contours.size();
}
int bot_front_color(){
          //cvtColor(original1,arena_calib, CV_BGR2GRAY );

          inRange(original1,Scalar(lowerh_bot_front,lowers_bot_front,lowerv_bot_front),Scalar(upperh_bot_front,uppers_bot_front,upperv_bot_front),arena_calib2);
          GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
          vector<Vec3f> circles;
           HoughCircles(arena_calib2, circles, CV_HOUGH_GRADIENT, 1, arena_calib2.rows/16, 200, 25, 10, 60);
           for( size_t i = 0; i < circles.size(); i++ )
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                std::cout << cvRound(circles[i][0])<< cvRound(circles[i][1]) <<std::endl;
               *&center_bot_frontx = cvRound(circles[i][0]);
               *&center_bot_backy = cvRound(circles[i][1]);

                // circle center
                circle(arena_calib2, center, 3, Scalar(0,255,0), -1, 8, 0 );
                // circle outline
                circle(arena_calib2, center, radius, Scalar(0,45,255), 3, 8, 0 );
            }

                imshow("Bot front Color Calibration", arena_calib2);

                return circles.size();
}
int bot_back_color(){
          //cvtColor(original1,arena_calib, CV_BGR2GRAY );

          inRange(original1,Scalar(lowerh_bot_back,lowers_bot_back,lowerv_bot_back),Scalar(upperh_bot_back,uppers_bot_back,upperv_bot_back),arena_calib);
          GaussianBlur(arena_calib2,arena_calib2, Size(9, 9), 2, 2 );
          vector<Vec3f> circles;
           HoughCircles(arena_calib2, circles, CV_HOUGH_GRADIENT, 1, arena_calib2.rows/16, 200, 25, 10, 60);
           for( size_t i = 0; i < circles.size(); i++ )
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
                std::cout << cvRound(circles[i][0])<< cvRound(circles[i][1]) <<std::endl;
               *&center_bot_backx = cvRound(circles[i][0]);
                *&center_bot_backy = cvRound(circles[i][1]);

                // circle center
                circle(arena_calib2, center, 3, Scalar(0,255,0), -1, 8, 0 );
                // circle outline
                circle(arena_calib2, center, radius, Scalar(0,45,255), 3, 8, 0 );
            }

                imshow("Bot back Color Calibration", arena_calib2);

                return circles.size();

}









