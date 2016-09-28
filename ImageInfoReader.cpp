#include "ImageInfoReader.h"
#include "exiv2/exiv2.hpp"
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
namespace qm{


ImageInfoReader::ImageInfoReader()
{

}

ImageInfoReader::~ImageInfoReader()
{

}

void ImageInfoReader::readExifInfo(QString &image_file)
{
	using namespace Exiv2;
	
	try
	{
		Image::AutoPtr image = ImageFactory::open(image_file.toStdString());
		if (image.get() == 0)
		{
			throw;
		}
		image->readMetadata();
		//image->Exif.Photo.FocalLengthIn35mmFilm Exif.Photo.FocalLength
		Exiv2::ExifData &exifData = image->exifData();
		auto rel = exifData.findKey(Exiv2::ExifKey("Exif.Image.Model"));
		if (rel != exifData.end())
		{
			camera_type_ = QString::fromStdString(rel->getValue()->toString());
		}
		else
		{
			camera_type_ = "custom";
		}

		bay_pat_ = "RVWB";

		rel = exifData.findKey(Exiv2::ExifKey("Exif.Image.Orientation"));
		if (rel != exifData.end())
		{
			if (rel->getValue()->toLong()!=1)
				throw QString("not set the image horizontal! ");
		}
		orientation_ = "Horizontal (normal)";

		rel = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLength"));
		if (rel != exifData.end())
		{
			focal_length_ = rel->getValue()->toFloat();
		}
		else
		{
			throw QString("no focal length!");
		}

		//rel = exifData.findKey(Exiv2::ExifKey("Exif.Photo.FocalLengthIn35mmFilm"));
		//if (rel != exifData.end())
		//{
		//	focal_length35_ = rel->getValue()->toFloat();
		//}
		//else
		//{
		//	focal_length35_ = focal_length_;
		//}

		width_ = exifData["Exif.Photo.PixelXDimension"].getValue()->toLong();
		height_ = exifData["Exif.Photo.PixelYDimension"].getValue()->toLong();
		
	}
	catch (...)
	{
		throw QString("can not open the file : " + image_file);
	}
	
}

void ImageInfoReader::writeToFile(QString &info_file)
{
	QFile xml_file(info_file);
	if (!xml_file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
		throw QString("can not create or open the file:" + info_file);
	QDomDocument doc;
	auto instruction = doc.createProcessingInstruction("xml", "version=\"1.0\"");
	doc.appendChild(instruction);
	QDomElement root = doc.createElement("XmlXifInfo");

	QDomElement hg = doc.createElement("HGrev");
	hg.appendChild(doc.createTextNode("-1"));
	root.appendChild(hg);
	
	QDomElement foc = doc.createElement("FocMM");
	foc.appendChild(doc.createTextNode(QString::number(focal_length_)));
	root.appendChild(foc);

	//QDomElement foc35 = doc.createElement("Foc35");
	//foc35.appendChild(doc.createTextNode(QString::number(focal_length35_)));
	//root.appendChild(foc35);

	QDomElement sz = doc.createElement("Sz");
	sz.appendChild(doc.createTextNode(QString::number(width_) + " " + QString::number(height_)));
	root.appendChild(sz);

	QDomElement cam = doc.createElement("Cam");
	cam.appendChild(doc.createTextNode(camera_type_));
	root.appendChild(cam);

	QDomElement bay_pat = doc.createElement("BayPat");
	bay_pat.appendChild(doc.createTextNode(bay_pat_));
	root.appendChild(bay_pat);

	QDomElement orientation = doc.createElement("Orientation");
	orientation.appendChild(doc.createTextNode(orientation_));
	root.appendChild(orientation);

	doc.appendChild(root);

	QTextStream steam(&xml_file);
	doc.save(steam, 4, doc.EncodingFromTextStream);
	xml_file.close();
}

}