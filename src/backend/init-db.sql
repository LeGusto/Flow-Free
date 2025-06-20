CREATE DOMAIN posint AS integer CHECK (VALUE >= 0);
CREATE TYPE cell_type AS ENUM ('source', 'blocking');

CREATE TABLE level (
    id SERIAL PRIMARY KEY,
    dimension_x POSINT NOT NULL,
    dimension_y POSINT NOT NULL
);

CREATE TABLE cells (
    id SERIAL PRIMARY KEY,
    x POSINT NOT NULL,
    y POSINT NOT NULL,
    level_id INTEGER NOT NULL REFERENCES level(id) ON DELETE CASCADE,
    cell_type cell_type NOT NULL, 
    UNIQUE (level_id, x, y)
);

CREATE TABLE source_cells (
    id INTEGER PRIMARY KEY REFERENCES cells(id) ON DELETE CASCADE,
    color_value POSINT NOT NULL
);

CREATE TABLE blocking_cells (
    id INTEGER PRIMARY KEY REFERENCES cells(id) ON DELETE CASCADE
);

CREATE FUNCTION check_coordinates_within_bounds() RETURNS trigger AS $$
BEGIN
  IF NEW.x >= (SELECT dimension_x FROM level WHERE id = NEW.level_id) OR
     NEW.y >= (SELECT dimension_y FROM level WHERE id = NEW.level_id) THEN
    RAISE EXCEPTION 'Cell coordinates (%s, %s) are out of bounds for level %s', NEW.x, NEW.y, NEW.level_id;
  END IF;
  RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_check_coordinates
BEFORE INSERT OR UPDATE ON cells
FOR EACH ROW EXECUTE FUNCTION check_coordinates_within_bounds();