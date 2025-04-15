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

    var valid_rules_with_strict: usize = 0;
    var valid_rules_with_loose: usize = 0;

    while (it.next()) |line| {
        var row = std.ArrayList(i32).init(allocator);
        defer row.deinit();

        var token_it = std.mem.splitScalar(u8, line, ' ');
        while (token_it.next()) |token| {
            if (token.len == 0) continue; // Skip empty tokens
            const value = try std.fmt.parseInt(i32, token, 10);
            try row.append(value);
        }

        if (validRow(row.items)) {
            valid_rules_with_loose = valid_rules_with_loose + 1;
            valid_rules_with_strict = valid_rules_with_strict + 1;
            continue;
        }

        for (0..row.items.len) |i| {
            if (validRow(removeElement(row, i))) {
                valid_rules_with_loose = valid_rules_with_loose + 1;
                break;
            }
        }
    }

    std.debug.print("Valid rules with strict: {}\n", .{valid_rules_with_strict});
    std.debug.print("Valid rules with loose: {}\n", .{valid_rules_with_loose});
}

fn removeElement(arr: std.ArrayList(i32), index: usize) []i32 {
    if (index >= arr.items.len) {
        return arr.items;
    }
    
    var temp = arr.allocator.alloc(i32, arr.items.len - 1) catch return &[_]i32{};
    
    var dest_index: usize = 0;
    for (0..arr.items.len) |i| {
        if (i != index) {
            temp[dest_index] = arr.items[i];
            dest_index += 1;
        }
    }
    
    return temp;
}

fn validRow(row: []i32) bool {
    var prev_status: ?i32 = null;

    if (row.len < 2) {
        return false;
    }

    for (0..row.len-1) |i| {
        var status: i8 = 0;

        if (row[i] == row[i + 1]) {
            status = 1;
        }

        if (row[i] > row[i + 1]) {
            status = 2;
        }

        if (prev_status == null) {
            prev_status = status;
        }

        const diff = row[i] - row[i + 1];

        const allowed_diff = (diff >= -3 and diff <= -1) or (diff >= 1 and diff <= 3);
        if (!allowed_diff) {
            return false;
        }

        if (status != prev_status) {
            return false;
        }
    }

    return true;
}

const std = @import("std");

/// This imports the separate module containing `root.zig`. Take a look in `build.zig` for details.
const lib = @import("_2_lib");
