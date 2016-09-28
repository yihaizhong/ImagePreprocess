#ifndef IMAGEPREPROCESS_H
#define IMAGEPREPROCESS_H

#include "imagepreprocess_global.h"
#include <QVector>
class QString;
namespace qm{
class ImageInfoReader;
class PosDataset;

class DLL_EXPORT ImagePreprocess
{
public:
	ImagePreprocess();
	~ImagePreprocess();
	void set_image_files(QVector<QString> &image_files);
	virtual void setImageHorizontal();
	virtual void writeImageInfo(ImageInfoReader *reader);
	virtual void writeImagePair(PosDataset &pos_data_set);
	virtual void enhanceImage();
private:
	QVector<QString> image_files_;
};
}
#endif // IMAGEPREPROCESS_H
