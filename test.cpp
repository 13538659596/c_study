#include "test.h"
#include "commen.h"
#include <android/log.h>
#include <stdlib.h>
#define LOG_TAG "Test_Jni"

//���ڴ�ӡdebug�����log��Ϣ
//__VA_ARGS__ �ɱ����
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

//���ڴ�ӡinfo�����log��Ϣ
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

//���ڴ�ӡerror�����log��Ϣ
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

JNIEXPORT jstring JNICALL Java_com_example_jnitest_Jclass_getStringFromC(
		JNIEnv *env, jclass cls) {

	const char *c_str = "hello word im cpp";

	return env->NewStringUTF(c_str); /* �����µ�Java.lang.String�����JVM�����ڴ�ʧ�ܣ����׳�OutOfMemoryError�����ҷ���NULL */
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
	 //4.�ͷ���Դ
	env->ReleaseIntArrayElements(jarry, arry, JNI_COMMIT);
	//    env->ReleaseIntArrayElements(jarr, arr, JNI_ABORT);
	//  �������һ������(���ָ��ָ�������Ϊ����ʱ������ò�����������)
	//      0       copy back the content and free the elems buffer
	//      JNI_COMMIT      copy back the content but do not free the elems buffer
	//      JNI_ABORT       free the buffer without copying back the possible changes

}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    createArrayMethod
 * Signature: (I)[I
 * java��c++���黥��ת��
 */
JNIEXPORT jintArray JNICALL Java_com_example_jnitest_Jclass_createArrayMethod(
JNIEnv * env, jobject jobj, jint lenth) {

	  //1.�½�����len����
	    jintArray jarr = env->NewIntArray(lenth);
	    //2.��ȡ����ָ��
	    jint *arr = env->GetIntArrayElements(jarr, JNI_FALSE);
	    //3.��ֵ

	    for(  int i = 0; i < lenth; i++){
	        arr[i] = i;
	    }
	    //4.ͬ�����鲢�ͷ�c/c++ �����ڴ�
	    env->ReleaseIntArrayElements(jarr, arr, 0);
	    //5.��������
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
 * ����java�Ǿ�̬����
 */
JNIEXPORT jstring JNICALL Java_com_example_jnitest_Jclass_accessFile2
  (JNIEnv * env, jobject jobj){
	//��ȡ�����
	jclass cls = env->GetObjectClass(jobj);
	//��ȡ����ID
	jfieldID  fieldId = env->GetFieldID(cls, "key",  "Ljava/lang/String;");
	//��ȡ����ֵ
	//ģʽ     get<type>Field
	jstring key = (jstring)env->GetObjectField(jobj, fieldId);
	//��jstringת����char*
	const char * cstr = env->GetStringUTFChars(key, JNI_FALSE);

	//�޸�ֵ
	char str[20] = "good";
	strcat(str, cstr);
	jstring newString = env->NewStringUTF(str);

	//��������ֵ
	//ģʽ     set<type>Field
	env->SetObjectField(jobj, fieldId, newString);

	return newString;
}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    setCount
 * Signature: ()V
 * ����java��̬����
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
 * ����java�Ǿ�̬����
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
 * ����java��̬����
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
//jni����Ĳ���
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_sort
  (JNIEnv *env, jobject jobj, jintArray arry){

	jint *jarry = env->GetIntArrayElements(arry, JNI_FALSE);
	int leng = env->GetArrayLength(arry);


	//  void qsort( void *buf, size_t num, size_t size, int (*compare)(const void *, const void *) );
	// ��buf ָ�������(����num ��,ÿ��Ĵ�СΪsize)���п��������������compare �ĵ�һ������С�ڵڶ������������ظ�ֵ��������ڷ�����ֵ��������ڷ�����ֵ��������buf ָ������ݰ���������
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
 * �ֲ��������ͷ�
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_deleteLocalValue
  (JNIEnv *env, jobject jobj) {

	jclass cls = env->FindClass("java/util/Date");

	jmethodID data_constructor = env->GetMethodID(cls, "<init>", "()V");

	for(int i = 0; i < 10; i++) {
		jobject obj = env->NewObject(cls, data_constructor);
		//��Date�����һϵ�в���

		//����java�����ڴ�
		env->DeleteLocalRef(obj);


		//������������õ�Date����
	}

}

jstring gloabl_str;

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    createGloablValue
 * Signature: ()V
 * ����ȫ�ֱ���
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
 * ��ȡȫ�ֱ���
 */
JNIEXPORT jstring JNICALL Java_com_example_jnitest_Jclass_getGloablValue
  (JNIEnv *env, jobject jobj) {
	return gloabl_str;
}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    deleteGloablValue
 * Signature: ()Ljava/lang/String;
 * �ͷ�ȫ�ֱ���
 */
JNIEXPORT jstring JNICALL Java_com_example_jnitest_Jclass_deleteGloablValue
  (JNIEnv *env, jobject jobj) {
	env->DeleteGlobalRef(gloabl_str);
}

/*
 * Class:     com_example_jnitest_Jclass
 * Method:    exception
 * Signature: ()V
 * �쳣����  JNI�Լ��׳�����Throwable�쳣��java����Exception��׽���� ������Throwable
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_exception
  (JNIEnv *env, jobject jobj){

	jclass cls = env->GetObjectClass(jobj);

	jfieldID key = env->GetFieldID(cls, "key1","Ljava/lang/String;");

	//ExceptionClear �����쳣������쳣����֤java������Լ�����
	jthrowable throwable = env->ExceptionOccurred();
	/*if(throwable != NULL) {
		LOGE("-----------------------------------------------------");
		env->ExceptionClear();
		return;
	}*/
	//�׳�java�����catch�����쳣
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
 * �ļ����
 */
JNIEXPORT void JNICALL Java_com_example_jnitest_Jclass_fileSplit
  (JNIEnv * env, jobject jobj, jstring file_path, jint spliteCount, jstring file_patten){

	const char *path = env->GetStringUTFChars(file_path, NULL);

	const char *patten = env->GetStringUTFChars(file_patten, NULL);

	//������ŷָ����ļ���
	char **newPath = (char **)malloc(spliteCount * sizeof(char*));
	for(int i = 0; i < spliteCount; i++) {
		newPath[i] = (char*)malloc(sizeof(char) * 50);
		sprintf(newPath[i], patten, (i+1));

		LOGE("%s", newPath[i]);
	}
	LOGE("filePath: %s", path);
	//���ļ�����ȡ�ļ���С
	FILE *fp = fopen(path, "r");
	if(fp == NULL) {
		jclass excp = env->FindClass("java/lang/NullPointerException");
		env->ThrowNew(excp, "no such file");
		return;
	}
	long file_size = getFileSize(fp);
	LOGE("filesize: %ld", file_size);
	//�ļ�������,ѭ����ȡ�ļ���д����Ҫ�ָ���ļ���
	if(file_size % spliteCount == 0) {
		int splite_size = file_size / spliteCount;

	}else{
		//��������  ��spliteCount - 1�� ǰ����ļ���СΪ  file_size / (spliteCount - 1), ���һ���ļ�Ϊ��Сfile_size % (spliteCount - 1)


	}

	//�ͷŷ�����ڴ�
	for(int i = 0; i < spliteCount; i++) {
		free(newPath[i]);
	}
	free(newPath);
	env->ReleaseStringUTFChars(file_path, path);
	env->ReleaseStringUTFChars(file_patten, path);

}



