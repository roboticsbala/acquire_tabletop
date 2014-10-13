/*
 * acquire_tabletop.h
 *
 *  Created on: Sep 29, 2014
 *      Author: ace
 */

#ifndef ACQUIRE_TABLETOP_H_
#define ACQUIRE_TABLETOP_H_

#include <ros/ros.h>
#include <ros/callback_queue.h>
#include <ros/callback_queue_interface.h>

#include <fstream>
#include <algorithm>

#include <std_msgs/String.h>

#include <cv_bridge/cv_bridge.h>
#include "opencv2/imgproc/imgproc.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <ros_over_peis/subscriber.h>
#include <ros_over_peis/publisher.h>

#include <doro_msgs/SiftDescriptor.h>
#include <doro_msgs/ClusterArray.h>
#include <doro_msgs/RecognizedObjectArray.h>

#include <acquire_tabletop/AcquireTabletop.h>
#include <cam_interface/cam_interface.h>

#include <tf/transform_listener.h>

#define IMAGES_SIZE 4

namespace acquire_tabletop {

class AcquireTabletopServer
{
protected:

	/**
	 * A variable to hold the subscription state.
	 */
	bool subscribed_;

	/**
	 * A common nodehandle.
	 */
	ros::NodeHandle nh_;

	/**
	 * A subscriber for the clusters.
	 */
	ros::Subscriber clusters_sub_;

	/**
	 * A variable to hold the clusters.
	 */
	doro_msgs::ClusterArray::Ptr clusters_ptr_;

	/**
	 * The server callback function.
	 */
	bool serverCB(AcquireTabletopRequest& request,
			AcquireTabletopResponse& response);

	/**
	 * The callback function for clusters.
	 */
	void clustersCB(const doro_msgs::ClusterArrayConstPtr& _clusters);

	/**
	 * A variable that holds the signatures that were retrived from CAM.
	 */
	doro_msgs::TableObjectArray cam_objects_;

	/**
	 * The Server.
	 */
	ros::ServiceServer server_;

	/**
	 * ROS Subscriber to the rgb/image_raw topic.
	 */
	ros::Subscriber image_raw_sub_;

	/**
	 * The actual message;
	 */
	sensor_msgs::ImagePtr image_;

	/**
	 * The array of descriptors from the image registry.
	 */
	std::vector < cv::Mat > descriptors_;

	/**
	 * Ids for the descriptors.
	 */
	std::vector <std::string> ids_;

	/**
	 * The successful matches.
	 */
	std::vector< std::vector<cv::DMatch> > matches_;

	/**
	 * The index of the image that has matched.
	 */
	std::vector<int> match_indices_;

	/**
	 * Callback to the image from rgb/image_raw.
	 */
	void imageCallback(const sensor_msgs::ImageConstPtr& _msg);

	/**
	 * Process image.
	 */
	std::string processImage(const cv::Mat& test_image, const float& tolerance);

	/**
	 * Prepare the set of descriptors from the image registry.
	 */
	void prepare();

	//void flase_prep();

	/**
	 * The array of images from the registry.
	 */
	//std::vector <cv::Mat> images_;

	/**
	 * The array of keypoints for the images from the registry.
	 */
	//std::vector <std::vector <cv::KeyPoint> > keypoints_;

	/**
	 * Fetch the descriptors from the peis tuple.
	 */
	void getDescriptorsFromTuples();

	/**
	 * Try to match the descriptors of the input image with those in the registry.
	 */
	void matchWithRegistry(const cv::Mat& _input_descriptors, const float& tolerance);

	/**
	 * Try to match the table_object with one in the CAM.
	 */
	void anchorUsingSignature(doro_msgs::TableObject& object, const float& tolerance);

	/**
	 * Cut image into pieces.
	 */
	cv::Mat cutImage(const cv::Mat& input_image, int row_offset, int col_offset, int row_size, int col_size);

public:

	boost::shared_ptr <tf::TransformListener> tf_listener_;

	/**
	 * Constructor.
	 */
	AcquireTabletopServer();

	/**
	 * Destructor.
	 */
	virtual ~AcquireTabletopServer();
};

} /* namespace acquire_tabletop */

#endif /* ACQUIRE_TABLETOP_H_ */