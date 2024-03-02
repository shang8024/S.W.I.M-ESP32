import numpy as np
import cv2
import sys
def polarCoorImage(source):
    # resize the image so that one of the larger dimension is 75
    # fill the rest with black
    if source.shape[0] > source.shape[1]:
        source = cv2.resize(source, (int(75*source.shape[1]/source.shape[0]), 75))
        # add black to the left and right, 255,255,255 is white
        print(source.shape)
        black = np.zeros((75, 75-source.shape[1], 3), dtype=np.uint8)
        print(source.shape)
        source = np.concatenate((black, source, black), axis=1)
    else:
        source = cv2.resize(source, (75, int(75*source.shape[0]/source.shape[1])))
        # add black to the top and bottom
        black = np.zeros((75-source.shape[0], 75, 3), dtype=np.uint8)
        source = np.concatenate((black, source, black), axis=0)
    # cut the image from the center to 75x75
    source = source[int((source.shape[0]-75)/2):int((source.shape[0]+75)/2), int((source.shape[1]-75)/2):int((source.shape[1]+75)/2)]
    #source = cv2.resize(source, (75, 75)) # Choose a size so that final image isn't cropped too much or too little, around 100x100 is good 

    # --- ensure image is of the type float ---
    img = source.astype(np.float32)
    #img = np.abs(img - 255) why revert the color???
    # --- the following holds the square root of the sum of squares of the image dimensions ---
    # --- this is done so that the entire width/height of the original image is used to express the complete circular range of the resulting polar image ---
    value = np.sqrt(((img.shape[0] / 2.0) ** 2.0) + ((img.shape[1] / 2.0) ** 2.0))

    polar_image = cv2.linearPolar(img, (img.shape[0] / 2, img.shape[1] / 2), value, cv2.WARP_FILL_OUTLIERS)

    polar_image = polar_image.astype(np.uint8)
    polar_image = polar_image[:, 3:75]
    # rotate the image
    polar_image = cv2.convertScaleAbs(polar_image, alpha=2)
    polar_image = cv2.rotate(polar_image, cv2.ROTATE_90_COUNTERCLOCKWISE)

    cv2.imshow("Polar Image", polar_image)
    cv2.imshow("OG", source)


    polar_image = np.ndarray.tolist(polar_image)
    # for the smallest items, convert it from [r,g,b] to 0xRRGGBB
    for i in range(len(polar_image)):
        for j in range(len(polar_image[i])):
                #convert [R,G,B] to 0xRRGGBB
                polar_image[i][j] = (polar_image[i][j][0]<<16) + (polar_image[i][j][1]<<8) + polar_image[i][j][2]
    string_img = str(polar_image)
    string_img = string_img.replace("], [", "},\n{")
    string_img = string_img.replace("[", "{")
    string_img = string_img.replace("]", "}")
    #print(string_img)
    # print(len(polar_image))
    # print(len(polar_image[0]))
    # print(len(polar_image[0][0]))

    cv2.waitKey(0)
    return string_img

def extractImages(vidcap):
    count = 0
    success,image = vidcap.read()
    success = True
    string = ""
    while success:
        vidcap.set(cv2.CAP_PROP_POS_MSEC,(count*300))    # 1 frame per second
        success,image = vidcap.read()
        # print ('Read a new frame: ', success)
        if not success:
            break
        print('Frame: ',count)
        tmp= "unsigned int img"+str(count)+"[row][col] = "
        string = string + tmp
        string = string + polarCoorImage(image)
        string = string + ";\n"
        count = count + 1
    # cut the last comma
    print('Total frames: ',count)
    return string, count

def checkFile(fileName):
    try:
        f = open(fileName)
        f.close()
    except IOError:
        print("File not accessible")
        sys.exit()
def clearAll(f):
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    f.close()
    sys.exit()


if __name__ == "__main__":
    if len(sys.argv) < 1:
        print("Please provide the file name")
        sys.exit()
    fileName=sys.argv[1]
    # check if it is a valid file
    checkFile(fileName)
    if len(sys.argv) > 2 and sys.argv[2] is not None:
        f = open(str(sys.argv[2])+".h", "w")
    else:
        f = open(str(fileName[:-4])+"_.txt", "w")
    f.write("#define POLAR_IMAGE\n")
    f.write("const int row = 72;\n")
    f.write("const int col = 75;\n")
    # check if it is a image or gif or vedio by checking the extension
    if fileName[-4:] == ".jpg" or fileName[-4:] == ".png":
        try:
            source = cv2.imread(fileName, 1)
            f.write("const int total_frame=1;\n")
            file_string = "unsigned int img0[row][col] = "+polarCoorImage(source)
            file_string = file_string + ";\n"
            f.write(file_string)
            file_string = "unsigned int *frames[1] = {&img0[0][0]};"
            f.write(file_string)
        except:
            print("Error in reading the image")
            clearAll(f)
    elif fileName[-4:] == ".gif" or fileName[-4:] == ".mp4":
        print(fileName)
        try:
            source = cv2.VideoCapture(fileName)
            file_string,frame_count = extractImages(source)
            res = "const int total_frame="+str(frame_count)+";\n"
            f.write(res)
            f.write(file_string)
            res = "unsigned int *frames["+str(frame_count)+"] = {"
            for i in range(frame_count):
                res = res + "&img"+str(i)+"[0][0],"
            res=res[:-1]
            res = res + "};"
            f.write(res)
        except:
            print("Error in reading the video")
            clearAll(f)
    print("Done")
    f.close()
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    