/**
 * @file      que_manager.cpp
 * @brief     Utility class for que management
 * @author    Vision Processing Community
 * @copyright 2016 Vision Processing Community. All rights reserved.
 */

#include "./que_manager.h"
#include <wx/thread.h>
#include <algorithm>
#include <utility>
#include <vector>

static wxMutex mutex_que_manager;

/**
 * @brief
 * Constructor.
 */
QueManager::QueManager(void) {
  // Initialize
  for (int i = 0; i < MAX_QUE_SIZE; i++) {
    cv::Mat* data = new cv::Mat();
    queue_data_.push_back(data);
  }
}

/**
 * @brief
 * Destructor.
 */
QueManager::~QueManager(void) {
  DEBUG_PRINT("QueManager::~QueManager\n");
  // Todo: call Delete
  DeleteQueue();
}

/**
 * @brief
 * Enqueue cv::Mat data.
 * @param enq_data [in] cv::Mat data.
 */
bool QueManager::Enqueue(cv::Mat* enq_data) {
  // mutex lock
  wxMutexLocker lock(mutex_que_manager);

  if ((queue_data_[kEnque]->size() != enq_data->size()) ||
      (queue_data_[kEnque]->type() != enq_data->type())) {
    DEBUG_PRINT("QueManager::Enqueue() modify image size\n");
    queue_data_[kEnque]->release();
    queue_data_[kEnque] = new cv::Mat(enq_data->size(), enq_data->type());
  }
  *queue_data_[kEnque] = enq_data->clone();

  std::swap(queue_data_[kEnque], queue_data_[kSwapque]);

  return true;
}

/**
 * @brief
 * Dequeue cv::Mat data.
 * @return cv::Mat data.
 */
cv::Mat* QueManager::Dequeue(void) {
  // mutex lock
  wxMutexLocker lock(mutex_que_manager);

  std::swap(queue_data_[kSwapque], queue_data_[kDeque]);

  return queue_data_[kDeque];
}

/**
 * @brief
 * Delete cv::Mat data queue.
 */
void QueManager::DeleteQueue() {
  DEBUG_PRINT("QueManager::DeleteQueue()\n");
  for (int i = 0; i < MAX_QUE_SIZE; i++) {
    if (queue_data_[i] != NULL) {
      queue_data_[i]->release();
    }
  }
}
