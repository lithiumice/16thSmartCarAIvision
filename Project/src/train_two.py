import keras
from keras.layers import Input, Conv2D, MaxPool2D, Dense, Activation, Flatten, AveragePooling2D, Deconvolution2D, Permute, Add
from keras.models import Model, Sequential 
from keras.optimizers import Adam, SGD
from keras.callbacks import EarlyStopping, ReduceLROnPlateau, ModelCheckpoint
from sklearn.model_selection import train_test_split 
from keras.utils import to_categorical
import cv2
from sklearn.utils import shuffle
import random
from PIL import Image

import os 
import numpy as np


# configs
#####################################
train_datat_path = "num_train/" #
test_data_path = "num_test/"   #
checkpoint_path = "checkpoint_two/model_number_para.ckpt" #
save_model_path = "checkpoint_two/model_number_para.h5"  #
epoch = 15  #
batch_size = 32  #
learning_rate = 0.001 #
pic_strong = False
#####################################




#定义训练数据加载函数
#加载数据的同时进行随机数据增强
def load_train_data(dir_path):
    images = []
    labels = []
    lab = os.listdir(dir_path)
    n=0
    for l in lab:
        img=os.listdir(dir_path+l)
        for i in img:
            img_path = dir_path+l+'/'+i
            if n %2 == 0:
                labels.append(int(0))
            else:
                labels.append(int(1))
            imgf = cv2.imread(img_path)
            #imgf_arr = imgf_arr.reshape(32,32,1)
            #print(imgf.shape[0],imgf.shape[1],imgf.shape[2])
            if pic_strong:
                random_int = random.randint(0,7)
                if random_int == 1:   #随机到0，7不变，随机到1水平翻转
                    imagef = Image.fromarray(cv2.cvtColor(imgf,cv2.COLOR_BGR2RGB))
                    imagef = imagef.transpose(Image.FLIP_LEFT_RIGHT)
                    imgf = cv2.cvtColor(np.array(imagef), cv2.COLOR_RGB2BGR)
                if random_int == 2:   #随机到2旋转5~15度角度
                    imagef = Image.fromarray(cv2.cvtColor(imgf,cv2.COLOR_BGR2RGB))
                    rand_r = random.randint(5,15)
                    imagef = imagef.rotate(rand_r)
                    imgf = cv2.cvtColor(np.array(imagef), cv2.COLOR_RGB2BGR)
                if random_int == 3:  #随机到3错切变换
                    imagef = Image.fromarray(cv2.cvtColor(imgf,cv2.COLOR_BGR2RGB))
                    rand_i = random.randint(0,1)
                    if rand_i == 0:
                        imagef = imagef.transform((32,32), Image.AFFINE, (1,-0.3,0,0,1,0), Image.BICUBIC, fill = 1)
                    else:
                        imagef = imagef.transform((32,32), Image.AFFINE, (1,0.3,0,0,1,0), Image.BICUBIC, fill = 1)
                    imgf = cv2.cvtColor(np.array(imagef), cv2.COLOR_RGB2BGR)
                if random_int == 4:  #随机到4进行随机比例缩放
                    imagef = Image.fromarray(cv2.cvtColor(imgf,cv2.COLOR_BGR2RGB))
                    rand_x = random.randint(3,9)
                    rand_y = random.randint(3,9)
                    imagef = imagef.transform((32,32), Image.AFFINE, (1,0,rand_x,0,1,rand_y), Image.BICUBIC, fill = 1)
                    imgf = cv2.cvtColor(np.array(imagef), cv2.COLOR_RGB2BGR)
                if random_int == 5:  #随机到5进行平移缩放
                    imagef = Image.fromarray(cv2.cvtColor(imgf,cv2.COLOR_BGR2RGB))
                    rand_x = random.randint(2,6)
                    rand_y = random.randint(2,6)
                    rand_x = rand_x/10
                    rand_y = rand_y/10
                    imagef = imagef.transform((32,32), Image.AFFINE, (rand_x,0,0,0,rand_y,0), Image.BICUBIC, fill = 1)
                    imgf = cv2.cvtColor(np.array(imagef), cv2.COLOR_RGB2BGR)
                if random_int == 6:  #随机到6加噪点，100黑100白
                    for r in range(0,25):
                        x = np.random.randint(0,32)
                        y = np.random.randint(0,32)
                        imgf[x,y,:] = 255
                    for r in range(0,25):
                        x = np.random.randint(0,32)
                        y = np.random.randint(0,32)
                        imgf[x,y,:] = 0
            imgf = imgf[...,(2,1,0)]
            images.append(imgf)
            imgf = 1
            
        n+=1
    return images,labels

#定义一个测试集数据加载函数
def load_test_data(dir_path):
    images = []
    labels = []
    lab = os.listdir(dir_path)
    n=0
    for l in lab:
        img=os.listdir(dir_path+l)
        for i in img:
            img_path = dir_path+l+'/'+i
            if n %2 == 0:
                labels.append(int(0))
            else:
                labels.append(int(1))
            imgf = cv2.imread(img_path)
            imgf = imgf[...,(2,1,0)]
            images.append(imgf)
            imgf = 1
            
        n+=1
    return images,labels


#数据预处理函数，将所有数据数组化，乱序
def prepare_data(images,labels,num_classes):
    train_x = np.array(images)
    train_x = np.float32(train_x)
    train_y = np.array(labels)
    indx = np.arange(0,train_y.shape[0])
    indx = shuffle(indx)
    train_x = train_x[indx]
    train_y = train_y[indx]
    train_y = to_categorical(train_y,num_classes)
    return train_x,train_y

pooling = MaxPool2D

def model():
    _in = Input(shape=(32,32,3))
    x = Conv2D(32, (3,3), padding='same')(_in)
    x = pooling((2,2))(x)
    x = Activation("relu")(x)

    x = Conv2D(64, (3,3), padding='same')(x)
    x = pooling((2,2))(x)
    x = Activation("relu")(x)

    x = Flatten()(x)
    x = Dense(2)(x)
    x = Activation("softmax")(x)

    return Model(_in, x)


#加载训练与测试数据
images_train,labels_train = load_train_data(train_datat_path)
images_test,labels_test = load_test_data(test_data_path)

#对加载的训练与测试数据进行预处理
train_x,train_y = prepare_data(images_train,labels_train,2)
test_x,test_y = prepare_data(images_test,labels_test,2)
for i in range(0,7604):
    train_x[i] = train_x[i]/128 - 1
for i in range(0,1000):
    test_x[i] = test_x[i]/128 - 1

model = model()

opt = Adam(lr=learning_rate)
model.compile(optimizer=opt, loss='categorical_crossentropy', metrics=["acc"])
early_stop = EarlyStopping(monitor = 'val_acc',patience=15, verbose = 1)
#reduce_lr = ReduceLROnPlateau(monitor = 'val_acc', patience=10,verbose = 1)
save_weights = ModelCheckpoint(checkpoint_path, 
                               save_best_only=True, monitor='val_acc')
#callbacks = [save_weights, reduce_lr, early_stop]
callbacks = [save_weights, early_stop]
if os.path.exists(checkpoint_path):
    model.load_weights(checkpoint_path)
    print("ok to load model !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
model.fit(train_x, train_y, epochs = epoch, batch_size = batch_size, 
          validation_data = (test_x, test_y), callbacks = callbacks)

model.save(save_model_path)

#创建一个文本文档，储存训练的参数

#file = open('./weights.txt', 'w')
#for v in model.trainable_variables:
    #file.wrtie(str(v.name) + '\n\n')
    #file.write(str(v.shape) + '\n\n')
    #file.write(str(v.numpy()) + '\n\n')
#file.close()
