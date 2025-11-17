from fastapi import FastAPI

# 1. Create a FastAPI instance
app = FastAPI()

# 2. Define a "route" or "endpoint"
# @app.get("/") tells FastAPI that the function below
# handles GET requests to the URL "/" (the root)
@app.get("/")
def read_root():
    return {"Hello": "World"}

# You can define more routes
@app.get("/items/{item_id}")
def read_item(item_id: int, q: str | None = None):
    # 'item_id' is a path parameter (part of the URL)
    # 'q' is an optional query parameter (e.g., /items/5?q=test)
    return {"item_id": item_id, "q": q}