#include <SimpleAmqpClient/SimpleAmqpClient.h>

#include <iostream>
#include <stdlib.h>


using namespace AmqpClient;

int main()
{
	char *localRabbit = getenv("AMQP_BROKER");
	Channel::ptr_t channel;
	if(localRabbit != NULL)
		channel = Channel::Create(localRabbit);
	else
		channel = Channel::Create();//default 127.0.0.1

	channel->DeclareQueue("queue_zx");
	channel->BindQueue("queue_zx", "amq.direct", "key_zx");

	BasicMessage::ptr_t msg_in = BasicMessage::Create();
	msg_in->Body("This is rabbit zx, How are you!");
	channel->BasicPublish("amq.direct", "key_zx", msg_in);
	channel->BasicConsume("queue_zx", "consumer_tag");

	BasicMessage::ptr_t msg_out = channel->BasicConsumeMessage("consumer_tag")->Message();

	std::cout<<"Message Received: "<<msg_out->Body()<<std::endl;
}
