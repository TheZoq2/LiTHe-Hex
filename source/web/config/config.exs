# This file is responsible for configuring your application
# and its dependencies with the aid of the Mix.Config module.
#
# This configuration file is loaded before any dependency and
# is restricted to this project.
use Mix.Config

# General application configuration
config :web,
  ecto_repos: [Web.Repo]

# Configures the endpoint
config :web, Web.Endpoint,
  url: [host: "localhost"],
  secret_key_base: "Q+JNAIiHMOpBCOnVcJcIKtRyVDLilv2bnz4VChxAJroUztwWp1/23snJYSBqyXsX",
  render_errors: [view: Web.ErrorView, accepts: ~w(html json)],
  pubsub: [name: Web.PubSub,
           adapter: Phoenix.PubSub.PG2]

# Configures the other endpoint
config :web, Web.CentralEndpoint,
  url: [host: "localhost"],
  secret_key_base: "Q+JNAIiHMOpBCOnVcJcIKtRyVDLilv2bnz4VChxAJroUztwWp1/23snJYSBqyXsX",
  render_errors: [view: Web.ErrorView, accepts: ~w(html json)],
  pubsub: [name: Web.PubSub,
           adapter: Web.PubSub.RabbitMQ]

# Configures Elixir's Logger
config :logger, :console,
  format: "$time $metadata[$level] $message\n",
  metadata: [:request_id]

# Import environment specific config. This must remain at the bottom
# of this file so it overrides the configuration defined above.
import_config "#{Mix.env}.exs"
