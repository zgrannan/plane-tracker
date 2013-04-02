#pragma once

#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace PlaneTracking {

    /**
     * Communcation between actors is done using `Message`s. Messages are immutable, and
     * their creation time is recorded.
     */

    struct Message {
      Message();
      const boost::ptime messageCreationTime;
    };

    template <typename T>
    struct Message<T> : public Message {
      Message(T data);
      const T data;
    };
}
