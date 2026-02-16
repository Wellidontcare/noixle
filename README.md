# noixle

Disclaimer:
This is a project I did for university. For serious image processing prototyping/exploration I would recommend python with napari, opencv and/or skimage and the other well documented image processing libraries :)

Here comes:

Imageprocessing with commandline-flavor.

Build on Qt and OpenCV with c++17ish

## build on linux
1. clone the project
2. cd inside the directory
3. make a new directory called build
4. run cmake . -B ./build
5. cd into build
6. run make
7. run the program with ./noixle

## build on windows
Generate buildfiles for visual studio with cmake

## usage
* help | shows this message
* echo  | displays a message to the user
* open \[file_path (optional)\] | opens an image
* save \[file_path (optional)\] | saves the currently opened image
* exit | exits the programm
* revert  | reverts back to last state
* clear | clear everything
* toggle_perf_measurement  | toggles the performance measurement
* history | shows the command history
* record \[start | stop\] | starts or stops the command recording
* load_macro  | loads and plays a recorded macro
* snapshot | saves the image as snapshot and displays it in a new tab-window
* load_snapshot \[snapshot_index\] | loads the selected snapshot or by provided snapshot index
* iminvert | inverts the currently opened image
* imfilter \['median' | 'gaussian' | 'bilateral' | 'sobel' | 'dilate' | 'erode' | 'laplace'\] | applies filter kernel to current image. no argument is custom Kernel
* imcconvert \[gray | color\] | converts the active image to the specified mode
* histogram \['cumulative'\] | displays a histogram of the currently viewed image
* imequalize | improves the image contrast by equalizing the histogram
* imgammacorrect \[gamma_value\]| corrects the gamme by the specified value
* imrotate [angle in deg] | rotates the active image by the given angle to the left
* impixelize [pixelsize] | pixelizes the active image
* imshadingcorrect | corrects the shading on an image
* imintegral | calcuates the integralimage of the active image
* imresize  [width, heigh] | resize image to give arguments
* sub [a b] | subtract b from a, image or scalar, i is active image, s marks snaphot images Example: sub s1 s2
* add [a b] | add b to a, image or scalar, i is active image, s marks snaphot images Example: add i 25
* mul [a b] | multiply a with b, image or scalar, i is active image, s marks snaphot images Example: mul s1 2
* div [a b] | divide a by b, image or scalar, i is active image, s marks snaphot images Example: div s2 s1
* imdft | shows the spectral domain of the image
* immerge [channel1 channel2 channel3] | merge channels, i is active image, s marks snaphot images Example: merge s0 s3
* gol | Do one iteration of conway's game of life, each pixel is a cell (Image has to be binarised)


* Use Tab to autocomplete!
* You can zoom with scrolling in and out while pressing Ctrl/Cmd or by using the + and - keys
* You can move the view while being zoomed in by pressing Ctrl/Cmd + Alt/Option and dragging the mouse
