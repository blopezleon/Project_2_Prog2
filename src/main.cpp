#include "TGAImage.h"
#include <iostream>
#include <sys/stat.h>
#include <direct.h>

void createOutputDirectory() {
    #ifdef _WIN32
    _mkdir("output");
    #else
    mkdir("output", 0777);
    #endif
}

void task1() {
    TGAImage layer1("input/layer1.tga");
    TGAImage pattern1("input/pattern1.tga");
    TGAImage result = TGAImage::multiply(layer1, pattern1);
    result.save("output/part1.tga");
}

void task2() {
    TGAImage layer2("input/layer2.tga");
    TGAImage car("input/car.tga");
    TGAImage result = TGAImage::subtract(car, layer2);
    result.save("output/part2.tga");
}

void task3() {
    TGAImage layer1("input/layer1.tga");
    TGAImage pattern2("input/pattern2.tga");
    TGAImage text("input/text.tga");
    
    TGAImage temp = TGAImage::multiply(layer1, pattern2);
    TGAImage result = TGAImage::screen(temp, text);
    result.save("output/part3.tga");
}

void task4() {
    TGAImage layer2("input/layer2.tga");
    TGAImage circles("input/circles.tga");
    TGAImage pattern2("input/pattern2.tga");
    
    TGAImage temp = TGAImage::multiply(layer2, circles);
    TGAImage result = TGAImage::subtract(temp, pattern2);
    result.save("output/part4.tga");
}

void task5() {
    TGAImage layer1("input/layer1.tga");
    TGAImage pattern1("input/pattern1.tga");
    TGAImage result = TGAImage::overlay(layer1, pattern1);
    result.save("output/part5.tga");
}

void task6() {
    TGAImage car("input/car.tga");
    car.addToChannel('G', 200);
    car.save("output/part6.tga");
}

void task7() {
    TGAImage car("input/car.tga");
    car.multiplyChannel('R', 4.0f);
    car.multiplyChannel('B', 0.0f);
    car.save("output/part7.tga");
}

void task8() {
    TGAImage car("input/car.tga");
    car.extractChannel('R').save("output/part8_r.tga");
    car.extractChannel('G').save("output/part8_g.tga");
    car.extractChannel('B').save("output/part8_b.tga");
}

void task9() {
    TGAImage result("input/layer_blue.tga"); 
    
    TGAImage red("input/layer_red.tga");
    TGAImage green("input/layer_green.tga");
    TGAImage blue("input/layer_blue.tga");
    
    result.setChannel('R', red);
    result.setChannel('G', green);
    result.setChannel('B', blue);
    
    result.save("output/part9.tga");
}

void task10() {
    TGAImage text("input/text2.tga");
    TGAImage result = text.rotate180();
    result.save("output/part10.tga");
}

int main() {
    try {
        createOutputDirectory();
        
        task1();
        task2();
        task3();
        task4();
        task5();
        task6();
        task7();
        task8();
        task9();
        task10();
        
        std::cout << "All tasks completed successfully!" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}