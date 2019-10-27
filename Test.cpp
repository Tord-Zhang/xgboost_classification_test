#include<iostream>
#include<opencv2/opencv.hpp>
#include<xgboost/c_api.h>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
using namespace std;
using namespace cv;


//you should get about 98% accuracy without modifying the code
int save(string pca_file, PCA* pca)
{
	FileStorage fs(pca_file, FileStorage::WRITE);
	fs << "mean" << pca->mean;
	fs << "e_vectors" << pca->eigenvectors;
	fs << "e_values" << pca->eigenvectors;
	fs.release();
	return 1;
}

int load(string pca_file, PCA* pca)
{
	FileStorage fs(pca_file, FileStorage::READ);
	fs["mean"] >> pca->mean;
	fs["e_vectors"] >> pca->eigenvectors;
	fs["e_values"] >> pca->eigenvectors;
	fs.release();
	return 1;
}

int main()
{

    BoosterHandle h_booster;
    const DMatrixHandle *g_trainHandle = nullptr;
    int g_trainHandleLength = 0;
    int res = XGBoosterCreate(g_trainHandle, g_trainHandleLength, &h_booster);
    res = XGBoosterLoadModel(h_booster, "xgboost.model");
    
    ifstream labellist("label.txt");
    vector<float> label;
    string line;
    int l;
    while(getline(labellist, line))
    {
         stringstream ss;
         ss << line;
         if(!ss.eof()){
             ss >> l;
	         label.push_back(float(l));
             label.push_back(float(l));   //nothing wrong here, should be pushed twice
          }
    }

    //labellist.close()   when this line or the following line is added, the prediction gets wrong
      
    FileStorage fs("mat.xml", FileStorage::READ);   //loading data
    Mat data;
    fs["feature"] >> data;

    const int rows = data.rows;
    const int cols = data.cols;

    float **train = new float *[rows];

    for (int i = 0; i < rows; i++)
	  train[i] = new float[cols];
    cout<<"start copying"<<endl;
    for(int i=0; i<rows; i++)
    {
        for(int j=0; j<cols; j++)
        {
    	    train[i][j] = data.at<float>(i,j);
        }
    }
    cout<<"mat to array finished"<<endl;
	// convert to DMatrix

	DMatrixHandle h_train[1], *h_test=nullptr;
	res = XGDMatrixCreateFromMat(reinterpret_cast<float *>(train), rows, cols, -1.0, &h_train[0]);

	bst_ulong out_len;
	const float *f;
	XGBoosterPredict(h_booster, h_train[0], 0,0,&out_len,&f);

    int count = 0;
	for (unsigned int i=0;i<out_len;i++)
    {
        cout<<f[i] <<"\t" << label[i]<<endl; //
	    if(f[i] == label[i]) count++;
    }
    cout<<"Accuracy: "<<float(count)/rows<<endl;

	XGDMatrixFree(h_train[0]);
	XGBoosterFree(h_booster);
    for(int i=0; i<rows; i++) 
        delete train[i];
    delete train;
	return 0;
}
