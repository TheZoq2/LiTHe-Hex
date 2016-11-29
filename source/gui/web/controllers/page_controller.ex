defmodule GUI.PageController do
  use GUI.Web, :controller

  def index(conn, _params) do
    render conn, "index.html"
  end
end
