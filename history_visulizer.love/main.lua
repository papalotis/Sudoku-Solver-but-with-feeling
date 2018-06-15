-- get all lines from a file, returns an empty
-- list/table if the file does not exist
function lines_from(file)
    local lines = {}
    for line in io.lines(file) do
        lines[#lines + 1] = line
    end
    return lines
end

function draw_sudoku_from_string(str)
    local char_table = {}
    str:gsub(
        ".",
        function(c)
            table.insert(char_table, c)
        end
    )

    local cell_w, cell_h = love.graphics.getWidth() / 9, love.graphics.getHeight() / 9

    for i, c in ipairs(char_table) do
        index = i - 1
        local x, y = index % 9, math.floor(index / 9)

        love.graphics.rectangle("line", x * cell_w, y * cell_h, cell_w, cell_h)

        if (x % 3 == 0 and y % 3 == 0) then
            local old_width = love.graphics.getLineWidth()
            love.graphics.setLineWidth(4)
            love.graphics.rectangle("line", x * cell_w, y * cell_h, cell_w * 3, cell_h * 3)
            love.graphics.setLineWidth(old_width)
        end

        if (c ~= "0") then
            love.graphics.printf(c, x * cell_w, y * cell_h, cell_w, "center", 0, 1, 1, 0, -cell_h / 3)
        end
    end
end

function love.load()
    love.graphics.setFont(love.graphics.newFont(24))
    love.window.setMode(720, 720)
    filename = arg[2]
    print(filename)
    counter = 1
    lines = lines_from(filename)
end

function love.draw()
    draw_sudoku_from_string(lines[counter])

    counter = math.min(counter + 1, #lines)
end
