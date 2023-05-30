#include "Map-jni.h"
#include "map.h"
#include "main.h"
#include <cstdlib>
#include <ctime>

JNIEXPORT void JNICALL Java_Map_init	(JNIEnv *env, jclass cls) {
	srand(time(NULL));
}

void JNICALL Java_Map_generate(JNIEnv *env, jclass cls, jint level, jintArray mapJArray, jintArray answerJArray) {
	jint *mapJint = env->GetIntArrayElements(mapJArray, NULL);
	jint *answerJint = env->GetIntArrayElements(answerJArray, NULL);

	int map[81] = {}, answer[81] = {};
	generate(map, answer, level);

	for (int i=0; i<81; ++i) {
		mapJint[i] = map[i];
		answerJint[i] = answer[i];
	}

	env->ReleaseIntArrayElements(mapJArray, mapJint, 0);
	env->ReleaseIntArrayElements(answerJArray, answerJint, 0);
}

JNIEXPORT jint JNICALL Java_Map_solve(JNIEnv *env, jclass cls, jintArray mapJArray, jboolean multiSolutionSearch, jboolean keepOutcome) {
	jint* mapJint = env->GetIntArrayElements(mapJArray, NULL);

	int map[81];

	for (int i=0; i<81; ++i) {
		map[i] = mapJint[i];
	}

	jint r = solve(map, multiSolutionSearch, keepOutcome);

	for (int i=0; i<81; ++i) {
		mapJint[i] = map[i];
	}

	env->ReleaseIntArrayElements(mapJArray, mapJint, 0);

	return r;
}
