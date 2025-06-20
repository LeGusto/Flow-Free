# main.py
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from typing import Dict
import psycopg2
import os
from dotenv import load_dotenv

load_dotenv()

app = FastAPI()

# Pydantic Models
class LevelData(BaseModel):
# level
#  -dimensions
#   -x
#   -y
#  -sources
#   -source
#    -color
#     -colorcode
#    -coords
#     -x
#     -y
#  -blockers
#   -blocker
#    -x
#    -y
    dimensions: Dict[str, int]
    sources: Dict[str, Dict[str, Dict[str, int]]]
    blockers: Dict[str, Dict[str, int]]

class Level(BaseModel):
    name: str
    level_data: LevelData
    creator: str

# Database connection
def get_db():
    conn = psycopg2.connect(
        host=os.getenv("DB_HOST"),
        database=os.getenv("DB_NAME"),
        user=os.getenv("DB_USER"),
        password=os.getenv("DB_PASS")
    )
    return conn

@app.post("/levels/")
async def create_level(level: Level):
    conn = None
    try:
        conn = get_db()
        cur = conn.cursor()
  
        cur.execute(
            "INSERT INTO levels (name, level_data, creator) VALUES (%s, %s, %s) RETURNING id",
            (level.name, level.level_data.model_dump_json(), level.creator)
        )
        level_id = cur.fetchone()[0]
        conn.commit()
        
        return {"id": level_id, **level.model_dump()}
        
    except Exception as e:
        if conn: conn.rollback()
        raise HTTPException(status_code=400, detail=str(e))
    finally:
        if conn: conn.close()

@app.get("/levels/{level_id}")
async def get_level(level_id: int):
    conn = None
    try:
        conn = get_db()
        cur = conn.cursor()
        
        cur.execute(
            "SELECT id, name, level_data, creator, created_at FROM levels WHERE id = %s",
            (level_id,)
        )
        level = cur.fetchone()
        
        if not level:
            raise HTTPException(status_code=404, detail="Level not found")
            
        return {
            "id": level[0],
            "name": level[1],
            "level_data": level[2],
            "creator": level[3],
            "created_at": level[4]
        }
        
    except Exception as e:
        raise HTTPException(status_code=400, detail=str(e))
    finally:
        if conn: conn.close()