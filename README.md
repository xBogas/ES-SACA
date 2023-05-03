# SACA-Sistema Automático de Classificação de Alvos


## Dependencies
* [OpenCV 4.7](https://docs.opencv.org/4.7.0/df/d65/tutorial_table_of_content_introduction.html)
* [Eigen lib](https://eigen.tuxfamily.org/index.php?title=Main_Page)

### Examples
- [Tutoriais](https://docs.opencv.org/4.7.0/d7/da8/tutorial_table_of_content_imgproc.html)

### Installing QT
- See https://web.stanford.edu/dept/cs_edu/resources/qt/install-linux to install is just follow this steps;
- When this error shows: "Could not load the Qt platform plugin "xcb" in "" even though it was found.", run this code 
```sh
sudo apt-get install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev
```

# Compile and Run the code
- Go to the repository where CMakeLists.txt is located
- Type the command: ```sh cmake -Bbuild -H. ```
- This will generate a build folder with the makefile inside
- Go to build folder: ```cd build```
- Insert the command ```make -j2``` to create the executable
- Finally run the executable: ```./NAME_OF_EXECUTABLE```

# Installing postgresql and libpqxx
- In the lubuntu terminal run the following commands: 
- ```sh 
  sudo apt-get install postgresql" to install postgresql
  sudo apt-get install libpqxx-dev
  ```
- To install libpqxx, the c++ library for postgresql database connection.

# Database management
//create database
sudo apt update
sudo apt install postgresql 
sudo service postgresql start
createdb es_saca
psql es_saca

//access database
sudo su postgres
psql es_saca

//paste format
once inside the database, paste the create_tables.sql content

//change password
sudo su postgres
psql
\password postgres