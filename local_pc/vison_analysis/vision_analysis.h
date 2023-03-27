#ifndef VISION_ANALYSIS_H
#define VISION_ANALYSIS_H

#include <QObject>
#include <opencv2/opencv.hpp>

class Detector : public QObject
{
    Q_OBJECT

public:
    explicit Detector(QObject *parent = nullptr);
    void loadModel(const QString &modelFile);
    void setTargetSize(int width, int height);
    bool detect(cv::Mat &frame, int &score);

private:    
    cv::dnn::Net m_net;
    cv::Size m_targetSize;
    float m_threshold;
    cv::Scalar m_mean;
    bool m_isModelLoaded;

    cv::Mat preprocessImage(cv::Mat &image);
    int getScoreFromOutput(const cv::Mat &output);
};

#endif // DETECTOR_H
