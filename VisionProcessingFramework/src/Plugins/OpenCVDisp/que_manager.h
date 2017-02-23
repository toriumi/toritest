/**
 * @file      que_manager.h
 * @brief     Utility class for que management.
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#ifndef _QUE_MANAGER_H_
#define _QUE_MANAGER_H_

#include <vector>
#include "./include.h"

#define MAX_QUE_SIZE 3

typedef enum {
  kEnque = 0,
  kSwapque,
  kDeque,
} QUE_Type;

/**
 * @class QueManager
 * @brief Utility class for que management.
 */
class QueManager {
 private:
  std::vector<cv::Mat*> queue_data_;

 public:
  /**
   * @brief
   * Constructor.
   */
  QueManager(void);

  /**
   * @brief
   * Destructor.
   */
  ~QueManager(void);

  /**
   * @brief
   * Enqueue cv::Mat data.
   * @param enq_data [in] cv::Mat data.
   */
  bool Enqueue(cv::Mat* enq_data);

  /**
   * @brief
   * Dequeue cv::Mat data.
   * @return cv::Mat data.
   */
  cv::Mat* Dequeue(void);

  /**
   * @brief
   * Delete cv::Mat data queue.
   */
  void DeleteQueue();
};

#endif /* _QUE_MANAGER_H_ */
