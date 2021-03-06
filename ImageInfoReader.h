#ifndef IMAGEINFO_H
#define IMAGEINFO_H
#include <QString>
#include "imagepreprocess_global.h"

namespace qm{
class DLL_EXPORT ImageInfoReader
{
public:
	ImageInfoReader();
	~ImageInfoReader();
	QString get_image_file()const;
	QString get_camera_type()const;
	QString get_bay_pat()const;
	QString get_orientation()const;
	double get_focal_length()const;
	double get_focal_length35()const;
	int get_width()const;
	int get_height()const;
    /**
     * @brief readExifInfo,use exiv2 to read
     * @param image_file
     */
	virtual void readExifInfo(QString &image_file);
    /**
     * @brief writeToFile, the default is xml
     * @param info_file
     */
	virtual void writeToFile(QString &info_file);
protected:
	QString camera_type_;
	QString bay_pat_;
	QString orientation_;
	double focal_length_;
	double focal_length35_;
	int width_;
	int height_;
};
}
#endif // IMAGEINFO_H
