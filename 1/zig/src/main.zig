//! By convention, main.zig is where your main function lives in the case that
//! you are building an executable. If you are making a library, the convention
//! is to delete this file and start with root.zig instead.

pub fn main() !void {
    const allocator = std.heap.page_allocator;

    const file = try std.fs.cwd().openFile("input.txt", .{});
    defer file.close();

    const file_size = try file.getEndPos();
    const buffer = try allocator.alloc(u8, file_size);

    _ = try file.readAll(buffer);

    var it = std.mem.splitScalar(u8, buffer, '\n');
    var line_count: usize = 0;
    while (it.next()) |_| {
        line_count += 1;
    }

    if (line_count == 0) {
        std.debug.print("Error: No lines found in the input file.\n", .{});
        return;
    }

    it.reset();

    const first_row: []i32 = try allocator.alloc(i32, line_count);
    defer allocator.free(first_row);
    const second_row: []i32 = try allocator.alloc(i32, line_count);
    defer allocator.free(second_row);

    var rowIndex: usize = 0;
    while (it.next()) |line| {
        var foundFirstRow = false;
        var row = std.mem.splitScalar(u8, line, ' ');
        while (row.next()) |word| {
            if (!foundFirstRow and word.len > 0) {
                const num = std.fmt.parseInt(i32, word, 10) catch |err| {
                    std.debug.print("Error parsing first row '{s}': {}\n", .{ word, err });
                    continue;
                };
                first_row[rowIndex] = num;
                foundFirstRow = true;
            } else if (word.len > 0) {
                const num = std.fmt.parseInt(i32, word, 10) catch |err| {
                    std.debug.print("Error parsing second row '{s}': {}\n", .{ word, err });
                    continue;
                };
                second_row[rowIndex] = num;
            }
        }
        rowIndex += 1;
    }

    if (rowIndex != line_count) {
        std.debug.print("Error: Expected {} rows, but found {}.\n", .{ line_count, rowIndex });
        return;
    }

    std.mem.sort(i32, first_row, {}, comptime std.sort.asc(i32));
    std.mem.sort(i32, second_row, {}, comptime std.sort.asc(i32));

    var diff: i32 = 0;
    var similarity: i32 = 0;
    for (first_row, 0..) |row, i| {
        similarity += count(row, second_row) * row;
        var local_diff = row - second_row[i];
        if (local_diff < 0) {
            local_diff *= -1;
        }

        diff += local_diff;
    }

    std.debug.print("Diff: {}\n", .{diff});
    std.debug.print("Similarity: {}\n", .{similarity});
}

fn count(needle: i32, haystack: []i32) i32 {
    var count_of_needle: i32 = 0;
    for (haystack) |item| {
        if (item == needle) {
            count_of_needle += 1;
        }
    }
    return count_of_needle;
}

const std = @import("std");

/// This imports the separate module containing `root.zig`. Take a look in `build.zig` for details.
const lib = @import("zig_lib");
