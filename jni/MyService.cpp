#include <string.h>
#include <jni.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>

#include <stdlib.h>
#include <sys/resource.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <android/log.h>
#define FLAG_FILE "/sdcard/gwlibrary/local/tmp/tmp.lock" //�ж��Ƿ��Ѿ����ػ�����������
#define LOG_TAG    "myndktest -- JNILOG" // ������Զ����LOG�ı�ʶ
//#undef LOG // ȡ��Ĭ�ϵ�LOG
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG, __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG, __VA_ARGS__)
#define LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG, __VA_ARGS__)
#define LOGF(...)  __android_log_print(ANDROID_LOG_FATAL,LOG_TAG, __VA_ARGS__)

jstring Java_com_example_myservice_MainActivity_stringFromJNI(JNIEnv* env, jclass thiz){
	LOGD("logd---string from MyNdkTest.cpp");
	return (jstring)env->NewStringUTF("string from MyNdkTest.cpp");
}

//�ж��Ƿ��Ѿ����ػ�����������
int isDeamonExist();
//ʹ��im startservice����������
void runProcess(const char* serviceName);
// ʹ��pidof���������Ŀ������Ƿ�����
int isProcessExist(const char *processName);

//�߳���
#define NUMTHREADS 1
//ȫ�ֱ���

JavaVM *g_jvm = NULL;
jobject g_obj = NULL;

void *thread_fun(void* arg){
	JNIEnv *env;
	jclass cls;
	jmethodID mid;


	if(g_jvm->AttachCurrentThread(&env,NULL) != JNI_OK){
		LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
		return NULL;
	}

	cls = env->GetObjectClass(g_obj);
	if (cls == NULL) {
		LOGE("FindClass() Error.....");
		goto error;
	}

	mid = env->GetStaticMethodID(cls,"fromJNI","(I)V");
	if (mid == NULL) {
		LOGE("GetMethodID() Error.....");
		goto error;
	}
	env->CallStaticVoidMethod(cls, mid,(int)arg);

	//============����һ����̨��ʬ����======================
	pid_t pid;
	struct rlimit r;
	int i;

	if((pid = fork()) < 0)
	{
		LOGI("i am deamon==1");
			perror("fork");
			exit(0);
	}
	else if(pid != 0)
	{
		LOGI("i am deamon==2");
	}

	setsid();
	LOGI("i am deamon==3");
	if((pid = fork()) < 0)
	{
		LOGI("i am deamon==4");
			perror("fork");
			exit(0);
	}
	else if(pid != 0)
	{
		LOGI("i am deamon==5");
	}

	chdir("/");
	LOGI("i am deamon==6");
	if(r.rlim_max == RLIM_INFINITY)
	{
			r.rlim_max = 1024;
	}
	//===================================

	int isExit;
	isExit = -3;
//	isExit = isDeamonExist();
	LOGI("%d isDeamonExist ============",isExit);
	if(isExit == -1){
		return NULL;
	}
	while(1)
	{
		LOGI("i am deamon sucess myservice====");
		isExit = isProcessExist("com.example.myservice");
		runProcess("com.example.myservice");
		LOGD("%d ============================", isExit);
		sleep(5);
	}

error:
	if (g_jvm->DetachCurrentThread() != JNI_OK) {
		LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
	}
	pthread_exit(0);
}


/**
 * @return �������� -1,û������0;
 */
int isDeamonExist(){
	int ret = -1;
	FILE * g_lockfile = NULL;

	//����Ƿ��Ѿ���һ��supervisor����������
	g_lockfile = fopen(FLAG_FILE, "a+");
	if (g_lockfile == NULL)
	{
		LOGE("fopen() failed:%s!\n", strerror(errno));
		return -1;
	}

	ret = flock(fileno(g_lockfile), LOCK_EX |LOCK_NB);
	if (ret != 0)
	{
		LOGE("this program already running\n");
		return -1;
	}
	return ret;
}

//ʹ��pidof���������Ŀ������Ƿ����У���ʱ�����⣬��֪����ôֻ�ܷ���0��
int isProcessExist(const char *processName)
{
	char buf[1024];
	char command[1024];
	FILE *fp;
	int ret = 0;
	sprintf(command, "pidof %s", processName);

	if ((fp = popen(command,"r")) == NULL)
	{
		LOGI("popen failed\n");
		exit(1);
	}

	if ((fgets(buf,1024,fp))!= NULL )
	{
		ret = 1;
		LOGI("pid is:%s\n", buf);
	}

	pclose(fp);
	return ret;
}

//ʹ��am����������android����
void runProcess(const char* serviceName)
{
	FILE *fp;
	char command[1024];
//	sprintf(command, "am startservice -n %s/%s", packageName, serviceName);
	sprintf(command, "am startservice -a com.example.myservice");
	LOGI("run cmd: %s\n", command);
	if ((fp = popen(command,"r")) == NULL)
	{
		LOGE("popen failed\n");
		exit(1);
	}else{
		LOGI("popen sucess\n");
	}

	pclose(fp);
}

JNIEXPORT void Java_com_example_myservice_MainActivity_mainThread(JNIEnv* env, jobject obj){
	int i;
	pthread_t pt[NUMTHREADS];

	for (i = 0; i < NUMTHREADS; i++) {
		pthread_create(&pt[i], NULL, &thread_fun, (void *)i);
	}
}

JNIEXPORT void Java_com_example_myservice_MainActivity_setJNIEnv(JNIEnv* env, jobject obj){
	env->GetJavaVM(&g_jvm);
	g_obj = env->NewGlobalRef(obj);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved){
	JNIEnv* env = NULL;
	jint result = -1;

	if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("GetEnv failed!");
		return result;
	}
	return JNI_VERSION_1_4;
}







#ifdef __cplusplus
}
#endif
