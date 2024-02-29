import numpy as np
import cv2
import sys
def polarCoorImage(source):
    source = cv2.resize(source, (72, 72)) # Choose a size so that final image isn't cropped too much or too little, around 100x100 is good 

    # --- ensure image is of the type float ---
    img = source.astype(np.float32)
    img = np.abs(img - 255)
    # --- the following holds the square root of the sum of squares of the image dimensions ---
    # --- this is done so that the entire width/height of the original image is used to express the complete circular range of the resulting polar image ---
    value = np.sqrt(((img.shape[0] / 2.0) ** 2.0) + ((img.shape[1] / 2.0) ** 2.0))

    polar_image = cv2.linearPolar(img, (img.shape[0] / 2, img.shape[1] / 2), value, cv2.WARP_FILL_OUTLIERS)

    polar_image = polar_image.astype(np.uint8)
    polar_image = polar_image[:, 3:75]

    cv2.imshow("Polar Image", polar_image)
    cv2.imshow("OG", source)


    polar_image = np.ndarray.tolist(polar_image)
    string_img = str(polar_image)
    string_img = string_img.replace("]], [[", "}},\n{{")
    string_img = string_img.replace("[", "{")
    string_img = string_img.replace("]", "}")
    #print(string_img)
    print(len(polar_image))
    print(len(polar_image[0]))
    print(len(polar_image[0][0]))

    cv2.waitKey(0)
    cv2.destroyAllWindows()
    return string_img

def extractImages(vidcap,isGif=False):
    count = 0
    success,image = vidcap.read()
    success = True
    string = ""
    while success:
        if not isGif:
            vidcap.set(cv2.CAP_PROP_POS_MSEC,(count*1000))    # 1 frame per second
        success,image = vidcap.read()
        print ('Read a new frame: ', success)
        if not success:
            break
        string = string + polarCoorImage(image)
        count = count + 1
        string = string + ","
    # cut the last comma
    string = string[:-1]
    string = "int img["+count+"][row][col][3] = {" + string
    string = string + "};"

def checkFile(fileName):
    try:
        f = open(fileName)
        f.close()
    except IOError:
        print("File not accessible")
        sys.exit()


if __name__ == "__main__":
    fileName=sys.argv[1]
    # check if it is a valid file
    checkFile(fileName)
    # check if it is a image or gif or vedio by checking the extension
    if fileName[-4:] == ".jpg" or fileName[-4:] == ".png":
        try:
            source = cv2.imread(fileName, 1)
            f = open(str(fileName)+"_.txt", "w")
            file_string = "int img[1][row][col][3] = {"+polarCoorImage(source)
            file_string = file_string + "};"
            f.write(file_string)
            f.close()
        except:
            print("Error in reading the image")
    elif fileName[-4:] == ".gif" or fileName[-4:] == ".mp4":
        try:
            source = cv2.VideoCapture(fileName, fileName[-4:] == ".gif")
            f = open(str(fileName)+"_.txt", "w")
            f.write(extractImages(source))
            f.close()
        except:
            print("Error in reading the video")
    else:
        print("Invalid file")