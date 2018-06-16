-- get all lines from a file, returns an empty
-- list/table if the file does not exist
function lines_from(file)
    local lines = {}
    for line in io.lines(file) do
        lines[#lines + 1] = line
    end
    return lines
end

table.print = function(self)
    str = "["
    for i, v in ipairs(self) do
        str = str .. tostring(v)
        if (i < #self) then
            str = str .. ", "
        end
    end
    str = str .. "]"
    print(str)
end

function string.toCharTable(self)
    local char_table = {}
    self:gsub(
        ".",
        function(c)
            table.insert(char_table, c)
        end
    )

    return char_table
end

function draw_sudoku_from_string(str)
    local char_table = str:toCharTable()

    fore_c = {114, 085, 034}
    back_c = {244, 189, 090}

    love.graphics.setBackgroundColor(back_c)
    love.graphics.setColor(fore_c)
    love.graphics.setLineWidth(3)

    local cell_w, cell_h = love.graphics.getWidth() / 9, love.graphics.getHeight() / 9

    for i, c in ipairs(char_table) do
        index = i - 1
        local x, y = index % 9, math.floor(index / 9)
        local gx, gy = x * cell_w, y * cell_h

        -- love.graphics.rectangle("line", x * cell_w, y * cell_h, cell_w, cell_h)
        love.graphics.circle("line", x * cell_w + cell_w / 2, y * cell_h + cell_h / 2, cell_w / 2)

        if (x % 3 == 0 and y % 3 == 0) then
            local old_width = love.graphics.getLineWidth()
            love.graphics.setLineWidth(6)

            love.graphics.arc("line", "open", gx + cell_w / 2, gy + cell_h / 2, cell_w / 2, -math.pi, -math.pi / 2)
            love.graphics.arc("line", "open", gx + cell_w * 2.5, gy + cell_h / 2, cell_w / 2, -math.pi / 2, 0)
            love.graphics.arc("line", "open", gx + cell_w * 2.5, gy + cell_h * 2.5, cell_w / 2, 0, math.pi / 2)
            love.graphics.arc("line", "open", gx + cell_w / 2, gy + cell_h * 2.5, cell_w / 2, math.pi / 2, math.pi)
            --top line
            love.graphics.line(gx + cell_w / 2, gy, gx + cell_w * 2.5, gy)
            --left
            love.graphics.line(gx, gy + cell_h / 2, gx, gy + cell_h * 2.5)
            --bottom
            love.graphics.line(gx + cell_w / 2, gy + cell_h * 3, gx + cell_w * 2.5, gy + cell_h * 3)
            love.graphics.line(gx + cell_w * 3, gy + cell_h / 2, gx + cell_w * 3, gy + cell_h * 2.5)

            -- love.graphics.rectangle("line", x * cell_w, y * cell_h, cell_w * 3, cell_h * 3)
            love.graphics.setLineWidth(old_width)
        end

        if (c ~= "0") then
            local h = love.graphics.getFont():getHeight()
            -- love.graphics.printf(c, x * cell_w, y * cell_h, cell_w, "center", 0, 1, 1, 0, -cell_h / 3)
            love.graphics.printf(c, gx, gy + (cell_h - h) / 2, cell_w, "center")
        end
    end
end

function love.textinput(t)
    counter = math.min(counter + 1, #lines)
end

function love.load()
    love.graphics.setFont(love.graphics.newFont("OpenSans-Light.ttf", 30))
    love.window.setMode(800, 800)
    filename = arg[2]
    counter = 1
    lines = lines_from(filename)
end

function love.draw()
    draw_sudoku_from_string(lines[counter])
end
