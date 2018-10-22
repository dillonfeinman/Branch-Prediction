#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <math.h>
using namespace std;

string predictTrue(string filename){
  string line;
  string ret;
  int numCorrect = 0;
  int numTotal = 0;
  ifstream myfile (filename);
  if(myfile.is_open()){
    while(getline(myfile,line)){
      numTotal += 1;
      istringstream iss(line);
      vector<string> results ((istream_iterator<string>{iss}),istream_iterator<string>());
      if(results.size() > 1){
        if(results[1] == "T"){
          numCorrect += 1;
        }
      }
    }
    ret = to_string(numCorrect) + "," + to_string(numTotal) + "\n";
    myfile.close();
  }
  return ret;
}
string predictFalse(string filename){
  string line;
  string ret;
  ifstream myfile (filename);
  int numCorrect = 0;
  int numTotal = 0;
  if(myfile.is_open()){
    while(getline(myfile,line)){
      numTotal += 1;
      istringstream iss(line);
      vector<string> results ((istream_iterator<string>{iss}),istream_iterator<string>());
      if(results.size() > 1){
        if(results[1] == "NT"){
          numCorrect += 1;
        }
      }
    }
    ret = to_string(numCorrect) + "," + to_string(numTotal) + ";\n";
    myfile.close();
  }
  return ret;
}
string predictBimodal1(string filename){
  string line;
  string ret;
  int sizes[] = {16, 32, 128, 256, 512, 1024, 2048};

  for(int i = 0; i < 7; i++){
    cout << sizes[i] << endl;
    ifstream myfile (filename);
    int numCorrect = 0;
    int numTotal = 0;
    if(myfile.is_open()){
      std::vector<string> predictor(sizes[i], "T");

      while(getline(myfile,line)){
        numTotal += 1;
        unsigned int addr;
        istringstream iss(line);
        vector<string> results ((istream_iterator<string>{iss}),istream_iterator<string>());
        stringstream ss;
        ss << hex << results[0];
        ss >> addr;
        int index = addr % sizes[i];
        if(predictor[index] == results[1]){
          numCorrect += 1;
        } else {
          predictor[index] = results[1];
        }
      }
    }
    myfile.close();
    ret += to_string(numCorrect) + "," + to_string(numTotal) + "; ";
  }
  ret += "\n";
  return ret;
}
string predictBimodal2(string filename){
  string line;
  string ret;
  int sizes[] = {16, 32, 128, 256, 512, 1024, 2048};
  for(int i = 0; i < 7; i++){
    int numCorrect = 0;
    int numTotal = 0;
    ifstream myfile (filename);

    if(myfile.is_open()){

      std::vector<int> predictor(sizes[i], 3);

      while(getline(myfile,line)){
        numTotal += 1;
        unsigned int addr;
        istringstream iss(line);
        vector<string> results ((istream_iterator<string>{iss}),istream_iterator<string>());
        stringstream ss;
        ss << hex << results[0];
        ss >> addr;
        int index = addr % sizes[i];
        if((predictor[index] == 2 || predictor[index] == 3) && results[1] == "T"){
          numCorrect += 1;
        }
        else if((predictor[index] == 0 || predictor[index] == 1) && results[1] == "NT"){
          numCorrect += 1;
        }
        if(results[1] == "T"){
          if(predictor[index] < 3){
            predictor[index] += 1;
          }
        } else {
          if(predictor[index] > 0){
            predictor[index] -= 1;
          }
        }
      }


    }
    myfile.close();
    ret += to_string(numCorrect) + "," + to_string(numTotal) + "; ";
  }
  ret += "\n";
  return ret;
}
string predictGshare(string filename){
  string line;
  string ret;
  int size = 2048;
  int numTotal = 0;
  int numCorrect = 0;

  unsigned int gr = 0;

  for(int i = 3; i < 12; i++){
    ifstream myfile (filename);
    if(myfile.is_open()){
      unsigned int bitSize = (int) pow(2, i) - 1;
      gr = 0;
      numTotal = 0;
      std::vector<int> predictor(size, 3);
      numCorrect = 0;
      while(getline(myfile,line)){
        numTotal += 1;
        unsigned int addr;
        istringstream iss(line);
        vector<string> results ((istream_iterator<string>{iss}),istream_iterator<string>());
        stringstream ss;
        ss << hex << results[0];
        ss >> addr;
        int index = (addr ^ (gr & bitSize)) % size;
        if((predictor[index] == 2 || predictor[index] == 3) && results[1] == "T"){
          numCorrect += 1;
        }
        else if((predictor[index] == 0 || predictor[index] == 1) && results[1] == "NT"){
          numCorrect += 1;
        }
        if(results[1] == "T"){
          if(predictor[index] < 3){
            predictor[index] += 1;
          }
        } else {
          if(predictor[index] > 0){
            predictor[index] -= 1;
          }
        }
        gr = gr << 1;
        if(results[1] == "T"){
          gr = gr | 1;
        } else {
          gr = gr | 0;
        }
      }
      myfile.close();
    }
    ret += to_string(numCorrect) + "," + to_string(numTotal) + "; ";
  }
  ret += "\n";
  return ret;
}
string predictTournament(string filename){
  string line;
  string ret;
  int size = 2048;
  int numTotal = 0;
  int numCorrect = 0;
  //gshare
  ifstream myfile (filename);
  if(myfile.is_open()){
    unsigned int bitSize = (int) pow(2, 11) - 1;
    unsigned long gr = 0;
    std::vector<int> gpredictor(size, 3);
    std::vector<int> bpredictor(size, 3);
    std::vector<int> tpredictor(size, 0);
    while(getline(myfile,line)){
      numTotal += 1;
      unsigned int addr;
      istringstream iss(line);
      vector<string> results ((istream_iterator<string>{iss}),istream_iterator<string>());
      stringstream ss;
      ss << hex << results[0];
      ss >> addr;
      unsigned int gindex = (addr ^ (gr & bitSize)) % size;
      unsigned int bindex = (addr % size);
      int gprediction = gpredictor[gindex];
      int bprediction = bpredictor[bindex];
      if(results[1] == "T"){
        if(bpredictor[bindex] < 3)
          bpredictor[bindex] += 1;
        if(gpredictor[gindex] < 3)
          gpredictor[gindex] += 1;
      } else {
        if(bpredictor[bindex] > 0)
          bpredictor[bindex] -= 1;
        if(gpredictor[gindex] > 0)
          gpredictor[gindex] -= 1;
      }
      gr = gr << 1;
      if(results[1] == "T"){
        gr = gr | 1;
      } else {
        gr = gr | 0;
      }
      if(((gprediction == 2 || gprediction == 3) && (bprediction == 2 || bprediction == 3)) && results[1] == "T"){
        numCorrect += 1;

      } else if((gprediction == 0 || gprediction == 1) && (bprediction == 0 || bprediction == 1) && results[1] == "NT"){
        numCorrect += 1;

      } else {
        if(tpredictor[bindex] == 0 || tpredictor[bindex] == 1){
          if(results[1] == "T" && (gprediction == 2 || gprediction == 3)){
            numCorrect += 1;

            if(tpredictor[bindex] > 0){
              tpredictor[bindex] -= 1;
            }
          }
          else if(results[1] == "NT" && (gprediction == 0 || gprediction == 1)){
            numCorrect += 1;

            if(tpredictor[bindex] > 0){
              tpredictor[bindex] -= 1;
            }
          }
          else if(results[1] == "NT" && (bprediction == 0 || bprediction == 1)){
            if(tpredictor[bindex] < 3){
              tpredictor[bindex] += 1;
            }
          } else if(results[1] == "T" && (bprediction == 2 || bprediction == 3)){
            if(tpredictor[bindex] < 3){
              tpredictor[bindex] += 1;
            }
          }
        } else {
          if(results[1] == "T" && (bprediction == 2 || bprediction == 3)){
            numCorrect += 1;

            if(tpredictor[bindex] < 3){
              tpredictor[bindex] += 1;
            }
          }
          else if(results[1] == "NT" && (bprediction == 0 || bprediction == 1)){
            numCorrect += 1;

            if(tpredictor[bindex] < 3){
              tpredictor[bindex] += 1;
            }
          }
          else if(results[1] == "T" && (gprediction == 2 || gprediction == 3)){
            if(tpredictor[bindex] > 0){
              tpredictor[bindex] -= 1;
            }
          }
          else if(results[1] == "NT" && (gprediction == 0 || gprediction == 1)){
            if(tpredictor[bindex] > 0){
              tpredictor[bindex] -= 1;
            }
          }
        }
      }
    }
    myfile.close();
  }
  ret += to_string(numCorrect) + "," + to_string(numTotal) + "; ";
  ret += "\n";
  return ret;
}

int main(int argc, char * argv[]){
  if(argc != 3){
    cerr << argc;
    cerr << "./predictors input_trace.txt output.txt" << endl;
  } else {
    string filename = argv[1];
    string allTrue = predictTrue(filename);
    cout << "True" << endl;
    string allFalse = predictFalse(filename);
    cout << "False" << endl;
    string bimodal1 = predictBimodal1(filename);
    cout << "B1" << endl;
    string bimodal2 = predictBimodal2(filename);
    cout << "B2" << endl;
    string gshare = predictGshare(filename);
    cout << "G" << endl;
    string tourn = predictTournament(filename);
    cout << "Tourn" << endl;
    ofstream outfile;
    outfile.open(argv[2]);
    outfile << allTrue;
    outfile << allFalse;
    outfile << bimodal1;
    outfile << bimodal2;
    outfile << gshare;
    outfile << tourn;
    outfile.close();
  }

  return 0;
}
