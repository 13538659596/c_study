#include "test.h"
#include "commen.h"
#include <android/log.h>
#include <stdlib.h>
#define LOG_TAG "Test_Jni"

//用于打印debug级别的log信息
//__VA_ARGS__ 可变参数
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

//用于打印info级别的log信息
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

//用于打印error级别的log信息
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

JNIEXPORT jstring JNICALL Java_com_example_jnitest_Jclass_getStringFromC(
		JNIEnv *env, jclass cls) {

	const char *c_str = "hello word im cpp";

	return env->NewStringUTF(c_str); /* 构造新的Java.lang.String，如果JVM分配内存失败，则抛出OutOfMemoryError，并且返回NULL */
}

JNIEXPORT jint JNICALL Java_com_example_jnitest_Jclass_getIntFromC(JNIEnv * env,
		jobject jobj) {

	int i = 20;

	return i;
}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    passArrayMethod
 * Signature: ([I)V
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_passArrayMethod
  (JNIEnv * env, jobject jobj, jintArray jarry){

	jint *arry = env->GetIntArrayElements(jarry,NULL);
	int len = env->GetArrayLength(jarry);
	LOGE("%d   %d\n", *arry, len);

	for(int i = 0; i < len; i++, arry++) {
			LOGE("arry[%d]  %d\n", i, *arry);
	}
	 //4.释放资源
	env->ReleaseIntArrayElements(jarry, arry, JNI_COMMIT);
	//    env->ReleaseIntArrayElements(jarr, arr, JNI_ABORT);
	//  对于最后一个参数(如果指针指向的数组为副本时，否则该参数不起作用)
	//      0       copy back the content and free the elems buffer
	//      JNI_COMMIT      copy back the content but do not free the elems buffer
	//      JNI_ABORT       free the buffer without copying back the possible changes

}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    createArrayMethod
 * Signature: (I)[I
 * java与c++数组互相转换
 */
JNIEXPORT jintArray JNICALL Java_com_example_jnitest_Jclass_createArrayMethod(
JNIEnv * env, jobject jobj, jint lenth) {

	  //1.新建长度len数组
	    jintArray jarr = env->NewIntArray(lenth);
	    //2.获取数组指针
	    jint *arr = env->GetIntArrayElements(jarr, JNI_FALSE);
	    //3.赋值

	    for(  int i = 0; i < lenth; i++){
	        arr[i] = i;
	    }
	    //4.同步数组并释放c/c++ 数组内存
	    env->ReleaseIntArrayElements(jarr, arr, 0);
	    //5.返回数组
	    return jarr;
}


/*
 * Class:     com_example_jnitest_Jclass
 * Method:    accessFile
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_example_jnitest_Jclass_accessFile
  (JNIEnv * env, jclass jcls){

	//jfieldID fieldId = env->GetFieldID()

}


/*
 * Class:     com_example_jnitest_Jclass
 * Method:    accessFile2
 * Signature: ()Ljava/lang/String;
 * 访问java非静态属性
 */
JNIEXPORT jstring JNICALL Java_com_example_jnitest_Jclass_accessFile2
  (JNIEnv * env, jobject jobj){
	//获取类对象
	jclass cls = env->GetObjectClass(jobj);
	//获取属性ID
	jfieldID  fieldId = env->GetFieldID(cls, "key",  "Ljava/lang/String;");
	//获取属性值
	//模式     get<type>Field
	jstring key = (jstring)env->GetObjectField(jobj, fieldId);
	//将jstring转换成char*
	const char * cstr = env->GetStringUTFChars(key, JNI_FALSE);

	//修改值
	char str[20] = "good";
	strcat(str, cstr);
	jstring newString = env->NewStringUTF(str);

	//设置属性值
	//模式     set<type>Field
	env->SetObjectField(jobj, fieldId, newString);

	return newString;
}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    setCount
 * Signature: ()V
 * 访问java静态属性
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_setCount
  (JNIEnv * env, jobject jobj){
	jclass cls = env->GetObjectClass(jobj);

	jfieldID fieldId = env->GetStaticFieldID(cls, "count", "I");
	jint count = (jint)env->GetStaticIntField(cls, fieldId);
	count++;
	env->SetStaticIntField(cls, fieldId, count);
}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    accessMethod
 * Signature: ()Ljava/lang/String;
 * 访问java非静态方法
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_accessMethod
  (JNIEnv * env, jobject jobj){


	jclass cls = env->GetObjectClass(jobj);


	//GetMethodID   GetFieldID
	jmethodID methodId = env->GetMethodID(cls, "getRandomInt", "(I)I");

	//CallMethodID    Get<tyep>Field
	jint random = env->CallIntMethod(jobj, methodId, 10);
	LOGE("%ld", random);
}


/*
 * Class:     com_example_jnitest_Jclass
 * Method:    accessStaticMethod
 * Signature: ()V
 * 访问java静态方法
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_accessStaticMethod
  (JNIEnv * env, jobject jobj) {
	jclass cls = env->GetObjectClass(jobj);

	jmethodID methodId = env->GetStaticMethodID(cls, "getUUID", "()Ljava/lang/String;");

	jstring uuid = (jstring)env->CallStaticObjectMethod(cls, methodId);
	const char * newString = env->GetStringUTFChars(uuid, JNI_FALSE);
	env->ReleaseStringUTFChars(uuid, newString);
	LOGE("uuid= %s", newString);
}



/*
 * Class:     com_example_jnitest_Jclass
 * Method:    sort
 * Signature: ([I)V
 */
//jni数组的操作
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_sort
  (JNIEnv *env, jobject jobj, jintArray arry){

	jint *jarry = env->GetIntArrayElements(arry, JNI_FALSE);
	int leng = env->GetArrayLength(arry);


	//  void qsort( void *buf, size_t num, size_t size, int (*compare)(const void *, const void *) );
	// 对buf 指向的数据(包含num 项,每项的大小为size)进行快速排序。如果函数compare 的第一个参数小于第二个参数，返回负值；如果等于返回零值；如果大于返回正值。函数对buf 指向的数据按升序排序。
	qsort(jarry, leng, sizeof(jint), compare);

	for(int i = 0; i < leng; i++, jarry++) {
			LOGE("----->  %d", *jarry);
		}

	jarry-=leng;
	env->ReleaseIntArrayElements(arry, jarry, 0);
}


/*
 * Class:     com_example_jnitest_Jclass
 * Method:    deleteLocalValue
 * Signature: ()V
 * 局部变量的释放
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_deleteLocalValue
  (JNIEnv *env, jobject jobj) {

	jclass cls = env->FindClass("java/util/Date");

	jmethodID data_constructor = env->GetMethodID(cls, "<init>", "()V");

	for(int i = 0; i < 10; i++) {
		jobject obj = env->NewObject(cls, data_constructor);
		//对Date对象的一系列操作

		//回收java对象内存
		env->DeleteLocalRef(obj);


		//下面操作无需用到Date对象
	}

}

jstring gloabl_str;

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    createGloablValue
 * Signature: ()V
 * 创建全局变量
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_createGloablValue
  (JNIEnv *env, jobject jobj) {
	const char *c_str = "hello_word";
	jstring jni_str = env->NewStringUTF(c_str);

	gloabl_str = (jstring)env->NewGlobalRef(jni_str);

}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    getGloablValue
 * Signature: ()Ljava/lang/String;
 * 获取全局变量
 */
JNIEXPORT jstring JNICALL Java_com_example_jnitest_Jclass_getGloablValue
  (JNIEnv *env, jobject jobj) {
	return gloabl_str;
}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    deleteGloablValue
 * Signature: ()Ljava/lang/String;
 * 释放全局变量
 */
JNIEXPORT jstring JNICALL Java_com_example_jnitest_Jclass_deleteGloablValue
  (JNIEnv *env, jobject jobj) {
	env->DeleteGlobalRef(gloabl_str);
}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    exception
 * Signature: ()V
 * 异常处理  JNI自己抛出的是Throwable异常，java端用Exception捕捉不到 必须用Throwable
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_exception
  (JNIEnv *env, jobject jobj){

	jclass cls = env->GetObjectClass(jobj);

	jfieldID key = env->GetFieldID(cls, "key1","Ljava/lang/String;");

	//ExceptionClear 发生异常后清除异常，保证java代码可以继续运
	jthrowable throwable = env->ExceptionOccurred();
	/*if(throwable != NULL) {
		LOGE("-----------------------------------------------------");
		env->ExceptionClear();
		return;
	}*/
	//抛出java层可以catch到的异常
	if(throwable != NULL) {
		LOGE("-----------------------------------------------------");
		env->ExceptionClear();
		jclass excp = env->FindClass("java/lang/NullPointerException");
		env->ThrowNew(excp, "no key1 field");
		return;
	}
	jstring key_str = (jstring)env->GetObjectField(jobj, key);

}


/*
 * Class:     com_example_jnitest_Jclass
 * Method:    fileMerge
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_fileMerge
  (JNIEnv *, jobject){

}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    fileSplit
 * Signature: (Ljava/lang/String;ILjava/lang/String;)V
 * 文件拆分
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_fileSplit
  (JNIEnv * env, jobject jobj, jstring file_path, jint spliteCount, jstring file_patten){

	const char *path = env->GetStringUTFChars(file_path, NULL);

	const char *patten = env->GetStringUTFChars(file_patten, NULL);

	//用来存放分割后的文件名
	char **newPath = (char **)malloc(spliteCount * sizeof(char*));
	for(int i = 0; i < spliteCount; i++) {
		newPath[i] = (char*)malloc(sizeof(char) * 50);
		sprintf(newPath[i], patten, (i+1));

		LOGE("%s", newPath[i]);
	}
	LOGE("filePath: %s", path);
	//打开文件，获取文件大小
	FILE *fp = fopen(path, "r");
	if(fp == NULL) {
		jclass excp = env->FindClass("java/lang/NullPointerException");
		env->ThrowNew(excp, "no such file");
		return;
	}
	long file_size = getFileSize(fp);
	LOGE("filesize: %ld", file_size);
	//文件能整除,循环读取文件，写入需要分割的文件中
	if(file_size % spliteCount == 0) {
		int splite_size = file_size / spliteCount;

	}else{
		//不能整除  （spliteCount - 1） 前面的文件大小为  file_size / (spliteCount - 1), 最后一个文件为大小file_size % (spliteCount - 1)


	}

	//释放分配的内存
	for(int i = 0; i < spliteCount; i++) {
		free(newPath[i]);
	}
	free(newPath);
	env->ReleaseStringUTFChars(file_path, path);
	env->ReleaseStringUTFChars(file_patten, path);

}



