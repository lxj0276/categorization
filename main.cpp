#include "textcategorization.h"
#include "../commom/func.h"
int main(int argc, char *argv[]) {
	classfy::Feature m_feature;
	m_feature.GetSample(argv[1], argv[2]);
	return 0;
}

