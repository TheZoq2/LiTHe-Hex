defmodule Web.PubSub.RabbitMQ do
  def start_link(name, options) do
    Supervisor.start_link(__MODULE__, {name, options},
      name: Module.concat(name, Supervisor))
  end

  def init({name, options}) do
  end
end
