// OpenCV Sample Application: facedetect.c

// Include header files
#include <map>
#include <string>

#include "cmd_arg_reader.h"

#include "cv.h"
#include "highgui.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <ctype.h>

using namespace std;

// Create memory for calculations
static CvMemStorage* storage = 0;

// Create a new Haar classifier
static CvHaarClassifierCascade* cascade = 0;

// Function prototype for detecting and drawing an object from an image
void detect_and_draw( IplImage* image, FILE *fp );

// Create a string that contains the cascade name
const char* c_cascade_name_map[] =
{"frontalface", "haarcascade_frontalface_alt.xml.gz",
"lefteye", "haarcascade_lefteye_2splits.xml.gz",
"nose", "haarcascade_mcs_nose.xml.gz",
"righteye", "haarcascade_mcs_righteye.xml.gz",
"profileface", "haarcascade_profileface.xml.gz",
"mouth", "haarcascade_mcs_mouth.xml.gz"
};

const char * cascade_name;

map<string, string> cascade_name_map;

/*    "haarcascade_profileface.xml";*/

// Main function, defines the entry point for the program.
int main( int argc, const char** argv )
{
	CmdArgReader::init(argc, argv, "-feature=frontalface");

    // Images to capture the frame from video or camera or from file
    IplImage *frame, *frame_copy = 0;

    // Input file name for avi or image file.
    const char* input_name;

    int i;

    for (i=0; i<6; i++)
    	cascade_name_map[string(c_cascade_name_map[i*2])] = string(c_cascade_name_map[i*2+1]);

    // Check for the correct usage of the command line
    if( argc == 0 )
    {
        fprintf( stderr,
        "Usage: facedetect input_filename [-feature=frontalface]\n" );
        return -1;
        /*input_name = argc > 1 ? argv[1] : 0;*/
    }

    cascade_name = GET_ARG_STRING( "feature" ).c_str();
    input_name = argv[1];


    // Load the HaarClassifierCascade
    cascade = (CvHaarClassifierCascade*)cvLoad( cascade_name_map[string(cascade_name)].c_str(), 0, 0, 0 );
    printf("cascade=%s\n", cascade_name_map[string(cascade_name)].c_str());
    
    // Check whether the cascade has loaded successfully. Else report and error and quit
    if( !cascade )
    {
        fprintf( stderr, "ERROR: Could not load classifier cascade\n" );
        return -1;
    }
    
    // Allocate the memory storage
    storage = cvCreateMemStorage(0);
    
    // Create a new named window with title: result
    cvNamedWindow( "result", 1 );

    // If loaded succesfully, then:
	// Assume the image to be lena.jpg, or the input_name specified
	const char* filename = input_name ? input_name : (char*)"lena.jpg";

	// Load the image from that filename
	IplImage* image = cvLoadImage( filename, 1 );

	// If Image is loaded succesfully, then:
	if( image )
	{
		// save file
		FILE *fp = fopen("faces.txt", "wt");

		// Detect and draw the face
		detect_and_draw( image, fp );

		// Wait for user input
		cvWaitKey(0);

		// Release the image memory
		cvReleaseImage( &image );

		fclose(fp);
	}


    
    // Destroy the window previously created with filename: "result"
    cvDestroyWindow("result");

    // return 0 to indicate successfull execution of the program
    return 0;
}

// Function to detect and draw any faces that is present in an image
void detect_and_draw( IplImage* img, FILE *fp )
{
    int scale = 1;

    // Create a new image based on the input image
    IplImage* temp = cvCreateImage( cvSize(img->width/scale,img->height/scale), 8, 3 );

    // Create two points to represent the face locations
    CvPoint pt1, pt2;
    int i;

    // Clear the memory storage which was used before
    cvClearMemStorage( storage );

    // Find whether the cascade is loaded, to find the faces. If yes, then:
    if( cascade )
    {

        // There can be more than one face in an image. So create a growable sequence of faces.
        // Detect the objects and store them in the sequence
        CvSeq* faces = cvHaarDetectObjects( img, cascade, storage,
                                            1.1, 4, CV_HAAR_DO_CANNY_PRUNING,
                                            cvSize(20, 20) );
        printf("Found faces: %d\n", faces->total);
        fprintf(fp, "%d\n", faces->total);

        // Loop the number of faces found.
        for( i = 0; i < (faces ? faces->total : 0); i++ )
        {
           // Create a new rectangle for drawing the face
            CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

            // Find the dimensions of the face,and scale it if necessary
            pt1.x = r->x*scale;
            pt2.x = (r->x+r->width)*scale;
            pt1.y = r->y*scale;
            pt2.y = (r->y+r->height)*scale;

            // Draw the rectangle in the input image
            cvRectangle( img, pt1, pt2, CV_RGB(255,0,0), 3, 8, 0 );

            fprintf(fp, "%d %d %d %d\n", pt1.x, pt1.y, pt2.x, pt2.y);
        }
    }

    // Show the image in the window named "result"
    cvShowImage( "result", img );

    // Release the temp image created.
    cvReleaseImage( &temp );
}

