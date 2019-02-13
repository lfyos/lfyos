#include "../driver.h" 
void wakeup_block(int block_id)
{
	int first_processor,first_semaphore;
	struct memory_block *b=block_id+(memory_body->memory_block);

	first_processor=b->sleep_semaphore.processor;
	first_semaphore=b->sleep_semaphore.semaphore;
	b->sleep_semaphore.processor=(-1);
	b->sleep_semaphore.semaphore=(-1);
	memory_wakeup(first_processor,first_semaphore); 
	return;
} 
