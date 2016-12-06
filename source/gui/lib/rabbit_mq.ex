defmodule GUI.RabbitMQ do
  def init() do
    {:ok, connection} = AMQP.Connection.open
    {:ok, to_pi} = AMQP.Channel.open(connection)
    {:ok, from_pi} = AMQP.Channel.open(connection)
    AMQP.Queue.declare(to_pi, "to_pi")
    AMQP.Queue.declare(from_pi, "from_pi")
    Agent.start_link(fn -> %{connection: connection, to_pi: to_pi}  end, name: __MODULE__)
    pid = spawn fn -> receive_message() end
    AMQP.Basic.consume(from_pi, "from_pi", pid, no_ack: true)
  end

  def send_message(message) do
    to_pi = Agent.get(__MODULE__, fn map -> map[:to_pi] end)
    AMQP.Basic.publish(to_pi, "", "to_pi", Poison.encode!(message))
  end

  def receive_message() do
    receive do
      {:basic_deliver, payload, _meta} ->
        IO.puts payload
        GUI.Endpoint.broadcast("client", "new_msg", Poison.decode!(payload))
        receive_message
    end
  end

  def shutdown() do
    connection = Agent.get(__MODULE__, fn map -> map[:connection] end)
    AMQP.Connection.close(connection)
  end
end
