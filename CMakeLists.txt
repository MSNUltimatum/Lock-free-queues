cmake_minimum_required(VERSION 3.17)
project(Lock_free_queue C)

set(CMAKE_C_STANDARD 11)


add_executable(Lock_free_queue main.c
        MSLFQ/MS_queue.c MSLFQ/MS_queue.h
        ConsumerProducer/Producer.c ConsumerProducer/Producer.h
        ConsumerProducer/Consumer.c ConsumerProducer/Consumer.h
        HelpStruct/queue_with_id.c HelpStruct/queue_with_id.h
        OptimisticLFQ/Optimistic_lock_free_queue.c OptimisticLFQ/Optimistic_lock_free_queue.h
        LFS/LFStack.c LFS/LFStack.h
        HelpStruct/HP.c HelpStruct/HP.h
        HelpStruct/list.c HelpStruct/list.h HelpStruct/exp_backoff.c HelpStruct/exp_backoff.h)
target_link_libraries(${PROJECT_NAME} pthread)
target_link_libraries(${PROJECT_NAME} atomic)