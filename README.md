# SACA-Sistema Automático de Classificação de Alvos


## Dependencies
* [OpenCV 4.7](https://docs.opencv.org/4.7.0/df/d65/tutorial_table_of_content_introduction.html)
* [Eigen lib](https://eigen.tuxfamily.org/index.php?title=Main_Page)

### Examples
- [Tutoriais](https://docs.opencv.org/4.7.0/d7/da8/tutorial_table_of_content_imgproc.html)

### Installing QT
- See https://web.stanford.edu/dept/cs_edu/resources/qt/install-linux to install is just follow this steps;
- When QT is installed, just change the path of QT in local_pc/src/CMakeLists.txt -> set(CMAKE_PREFIX_PATH "/YOUR_PATH/Qt/6.4.3/gcc_64")

# Compile and Run the code
- Go to the repository where CMakeLists.txt is located
- Type the command: "cmake -Bbuild -H."
- This will generate a build folder with the makefile inside
- Go to build folder: "cd build"
- Insert the command "make" to create the executable
- Finally run the executable: "./NAME_OF_EXECUTABLE"
