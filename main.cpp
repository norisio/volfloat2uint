// vim: set et sts=2 ts=2 sw=2 :
#include <iostream>
#include <iomanip>
#include <fstream>
using std::ios;

int main(int argc, char* argv[]){
  int const filesize = [&]() {
		std::ifstream r_file;
		r_file.open(argv[1], std::ios::binary);
		if(!r_file.is_open()){
      std::cerr << "cannot open file" << std::endl;
			exit(-1);
		}

		// ファイルサイズ計測
		r_file.seekg(0, std::ios::end);
		auto const eofpos = r_file.tellg();
		r_file.clear();
		r_file.seekg(0, std::ios::beg);
		auto const begpos = r_file.tellg();
		r_file.close();
    return eofpos - begpos;
	}();

  int const repeats = filesize / 4;

  std::ifstream r_file;
  std::ofstream outfile;

  r_file.open(argv[1], std::ios::binary);
  outfile.open("output_uint16.raw", ios::out|ios::binary|ios::trunc);
  if(!r_file.is_open() || !outfile.is_open()){
    std::cerr << "cannot open file" << std::endl;
    exit(-1);
  }

  std::cout << std::setprecision(5);
  for(int i=0; i<repeats; ++i){
    float f;
    r_file.read(reinterpret_cast<char*>(&f), sizeof(float));
    if(f<0.f){f=0.0f;}
    if(f>1.f){f=1.0f;}
    auto const u = static_cast<uint16_t>(f * 65535.0f);
#if 0
    if(u > 100){
      std::cout << f;
      std::cout << " -> " << u << std::endl;
    }
#endif
    outfile.write(reinterpret_cast<char const*>(&u), sizeof(uint16_t));
  }
  return 0;
}

