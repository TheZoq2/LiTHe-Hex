defmodule Web.RabbitMQ do
  def init() do
    {:ok, connection} = AMQP.Connection.open
    {:ok, to_pi} = AMQP.Channel.open(connection)
    {:ok, from_pi} = AMQP.Channel.open(connection)
    AMQP.Queue.declare(to_pi, "to_pi")
    AMQP.Queue.declare(from_pi, "from_pi")
    AMQP.Basic.consume(from_pi, "from_pi", nil, no_ack: true)
    spawn fn -> receive_message() end
  end

  def send_message(message) do
    AMQP.Basic.publish(to_pi, "", "to_pi", message)
  end

  def receive_message() do
    receive do
      {:basic_deliver, payload, _meta} ->
        Web.Endpoint.broadcast("client", "new:msg", payload)
        receive_message
    end
  end

  def shutdown() do
    AMQP.Connection.close(connection)
  end
end
