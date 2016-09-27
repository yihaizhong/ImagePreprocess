#ifndef IMAGEPREPROCESS_H
#define IMAGEPREPROCESS_H

#include "imagepreprocess_global.h"
template<class T>
class QVector;
class QString;
namespace qm{
class ImageInfo;
class PosDataset;
class DLL_EXPORT ImagePreprocess
{
public:
	ImagePreprocess();
	~ImagePreprocess();
	virtual void setImageHorizontal(QVector<QString> &image_files) = 0;
	virtual void writeImageInfo(QVector<ImageInfo> &image_info_vec) = 0;
	virtual void writeImagePair(PosDataset &pos_data_set) = 0;
	virtual void enhanceImage(QVector<QString> &image_files) = 0;
private:

};
}
#endif // IMAGEPREPROCESS_H
