#include "Image.hpp"
#include "Matrix.hpp"
#include "processing.hpp"
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
  string filename_in = argv[1];
  string filename_out = argv[2];
  int desired_width = atoi(argv[3]);
  int desired_height = 0;

  ifstream fin;
  fin.open(filename_in);
  if (!fin.is_open()) {
    cout << "Error opening file" << filename_in << endl;
    return 1;
  }

  Image *img = new Image;
  Image_init(img, fin);
  Matrix *energy = new Matrix;
  Matrix *cost = new Matrix;
  int seam[MAX_MATRIX_HEIGHT];

  if (argc == 4) {
    desired_height = Image_width(img);
  } else {
    desired_height = atoi(argv[4]);
  }

  if (argc != 4 && argc != 5) {
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
         << "WIDTH and HEIGHT must be less than or equal to original" << endl;
  }
  if (!(desired_width > 0 && desired_width <= Image_width(img))) {
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
         << "WIDTH and HEIGHT must be less than or equal to original" << endl;
  }
  if (!(desired_height > 0 && desired_height <= Image_height(img))) {
    cout << "Usage: resize.exe IN_FILENAME OUT_FILENAME WIDTH [HEIGHT]\n"
         << "WIDTH and HEIGHT must be less than or equal to original" << endl;
  }

  compute_energy_matrix(img, energy);
  compute_vertical_cost_matrix(energy, cost);
  find_minimal_vertical_seam(cost, seam);
  remove_vertical_seam(img, seam);
  seam_carve(img, desired_width, desired_height);

  ofstream fout;
  fout.open(filename_out);
  Image_print(img, fout);

  delete img;
  delete energy;
  delete cost;

  return 0;
}