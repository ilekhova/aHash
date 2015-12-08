#include <core/core.hpp>
#include <highgui/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <cv.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <fstream>
// ���������� ��� ��������
__int64 calcImageHash(IplImage* image, bool show_results=false);
// ������� ���������� ��������
__int64 calcHammingDistance(__int64 x, __int64 y);

int main(int argc, char* argv[])
{
        IplImage *object=0, *image=0;
         std::ifstream file("images.txt");
   std::vector<std::string> v; //������ �����
   std::string S;  //����������� ����� �� �����

   while(getline(file, S))  v.push_back(S);  //���������� � ������ �������� �� �����

   for (unsigned int i=0;i<v.size();i++) std::cout<<v.at(i)<<"\n"; //����� ������� �� �����

        char obj_name[] = "object.jpg";


        // ��� ������� ������� ������ ����������
        char* object_filename = argc >= 2 ? argv[1] : obj_name;
        object = cvLoadImage(object_filename, 1);
         __int64 hashO = calcImageHash(object, true);
        // ��� �������� ������� ������ ����������

        for (unsigned int i=0;i<v.size();i++)
        {
            int s = v[i].size();
             char img_name[s];
            for (int j = 0; j <s+1 ; j++)
            {
                img_name[j] = v[i][j];
            }

            char* image_filename = argc >= 3 ? argv[2] : img_name;
            image = cvLoadImage(image_filename, 1);
            printf("[i] object: %s\n", object_filename);
            printf("[i] image: %s\n", image_filename);

            if(!object){
                printf("[!] Error: cant load object image: %s\n", object_filename);
                return -1;
            }
            if(!image){
                printf("[!] Error: cant load test image: %s\n", image_filename);
                return -1;
            }
             __int64 hashI = calcImageHash(image, false);

        // ���������� ���������� ��������
        __int64 dist = calcHammingDistance(hashO, hashI);

        printf("[i] Hamming distance: %I64d \n\n\n", dist);
            cvReleaseImage(&image);

        }
        // �������� ��������





       /* // ������� �����������
        cvNamedWindow( "object");
        cvShowImage( "object", object );
        cvNamedWindow( "image");
        cvShowImage( "image", image );
        */
        // �������� ���

        //cvWaitKey(0);


        // ��� ������� �������
        //cvWaitKey(0);

        // ����������� �������


        cvReleaseImage(&object);
        // ������� ����
       // cvDestroyAllWindows();*/
        return 0;
}

// ���������� ��� ��������
__int64 calcImageHash(IplImage* src, bool show_results)
{
        if(!src){
                return 0;
        }

        IplImage *res=0, *gray=0, *bin =0;

        res = cvCreateImage( cvSize(8, 8), src->depth, src->nChannels);
        gray = cvCreateImage( cvSize(8, 8), IPL_DEPTH_8U, 1);
        bin = cvCreateImage( cvSize(8, 8), IPL_DEPTH_8U, 1);

        // ��������� ��������
        cvResize(src, res);
        // ��������� � �������� ������
        cvCvtColor(res, gray, CV_BGR2GRAY);
        // ��������� �������
        CvScalar average = cvAvg(gray);
        printf("[i] average: %.2f \n", average.val[0]);
        // ������� �������� ����������� ������������ ��������
        // ��� ����� ������������� ��������� ���������������
        cvThreshold(gray, bin, average.val[0], 255, CV_THRESH_BINARY);

        // �������� ���
        __int64 hash = 0;

        int i=0;
        // ����������� �� ���� �������� �����������
        for( int y=0; y<bin->height; y++ ) {
                uchar* ptr = (uchar*) (bin->imageData + y * bin->widthStep);
                for( int x=0; x<bin->width; x++ ) {
                        // 1 �����
                        if(ptr[x]){
                                // hash |= 1<<i;  // warning C4334: '<<' : result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)
                               hash |= (__int64)1<<i;
                        }
                        i++;
                }
        }

        printf("[i] hash: %I64X \n", hash);

       /* if(show_results){
                // ����������� �������� ��� ����������� �����������
                IplImage* dst3 = cvCreateImage( cvSize(128, 128), IPL_DEPTH_8U, 3);
                IplImage* dst1 = cvCreateImage( cvSize(128, 128), IPL_DEPTH_8U, 1);

                // ���������� ��������
                cvNamedWindow( "64");
                cvResize(res, dst3, CV_INTER_NN);
                cvShowImage( "64", dst3 );
                cvNamedWindow( "gray");
                cvResize(gray, dst1, CV_INTER_NN);
                cvShowImage( "gray", dst1 );
                cvNamedWindow( "bin");
                cvResize(bin, dst1, CV_INTER_NN);
                cvShowImage( "bin", dst1 );

                cvReleaseImage(&dst3);
                cvReleaseImage(&dst1);
        }*/

        // ����������� �������
        cvReleaseImage(&res);
        cvReleaseImage(&gray);
        cvReleaseImage(&bin);

        return hash;
}

// ������� ���������� �������� ����� ����� ������
// http://en.wikipedia.org/wiki/Hamming_distance
// http://ru.wikipedia.org/wiki/����������_��������
//
__int64 calcHammingDistance(__int64 x, __int64 y)
{
        __int64 dist = 0, val = x ^ y;

        // Count the number of set bits
        while(val)
        {
                ++dist;
                val &= val - 1;
        }

        return dist;
}
