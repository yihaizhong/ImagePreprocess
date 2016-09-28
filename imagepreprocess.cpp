#include "imagepreprocess.h"
#include <QVector>
#include <QDir>
#include <QFileInfo>

#include "exiv2/exiv2.hpp"
#include "ImageInfoReader.h"
namespace qm{
ImagePreprocess::ImagePreprocess() :image_files_(QVector<QString>())
{

}

ImagePreprocess::~ImagePreprocess()
{

}

void ImagePreprocess::set_image_files(QVector<QString> &image_files)
{
	image_files_ = image_files;
}

void ProcessOne(QString &file)
{
	using namespace Exiv2;
	try
	{
		Image::AutoPtr image = ImageFactory::open(file.toStdString());
		image->readMetadata();
		//image->
		Exiv2::ExifData &exifData = image->exifData();
		auto rel = exifData.findKey(Exiv2::ExifKey("Exif.Image.Orientation"));
		QFileInfo file_info(file);
		QString dir(file_info.absolutePath());
		QString bk_dir = dir + "/bk";
		QString file_name(file_info.fileName());
		if (rel != exifData.end())
		{
			if (rel->getValue()->toLong() != 1)
			{
				QDir().mkdir(bk_dir);
				//_mkdir((yt::getDir(file) + "\\bk").c_str());
				//rename(file.c_str(), (yt::getDir(file) + "\\bk\\" + yt::getFileName(file)).c_str());
				QFile::copy(file, bk_dir + "/" + file_name);
				exifData["Exif.Image.Orientation"].setValue("1");
				//image->setExifData(exifData);
				image->writeMetadata();
			}
		}
	}
	catch (...)
	{
		throw QString("can not open the image file: " + file);
	}

}
void ImagePreprocess::setImageHorizontal()
{
	//there can be multi-thread, if needed!
	for (auto bg = image_files_.begin(); bg != image_files_.end();++bg)
	{
		ProcessOne(*bg);
	}
}


void ImagePreprocess::writeImageInfo(ImageInfoReader *image_info_reader)
{
	QString first_image = image_files_[0];
	image_info_reader->readExifInfo(first_image);
	QFileInfo file_info(first_image);
	QString work_dir(file_info.absolutePath());
	QString tmp_dir(work_dir + "/Tmp-MM-Dir");
	QDir().mkdir(tmp_dir);
	for (auto bg = image_files_.begin(); bg != image_files_.end();++bg)
	{
		QString xml_file_name = QFileInfo(*bg).fileName() + "-MTD-4227.xml";
		xml_file_name.push_front(tmp_dir + "/");
		image_info_reader->writeToFile(xml_file_name);
	}
}

void ImagePreprocess::writeImagePair(PosDataset &pos_data_set)
{

}

void ImagePreprocess::enhanceImage()
{

}

}
