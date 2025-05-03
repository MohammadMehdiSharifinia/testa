import turtle

def draw_polygon(sides, length):
    """
    رسم یک چندضلعی با تعداد اضلاع و طول مشخص.
    
    :param sides: تعداد اضلاع
    :param length: طول هر ضلع
    :return: لیستی از موقعیت رأس‌ها و زوایای بیرونی
    """
    angle = 360 / sides
    external_angle = 180 - angle  # زاویه بیرونی
    positions = []

    for _ in range(sides):
        positions.append((turtle.pos(), turtle.heading() + external_angle))  # ذخیره موقعیت و زاویه بیرونی
        turtle.forward(length)
        turtle.left(angle)

    return positions

def draw_attached_polygons(base_positions, sides, length):
    """
    رسم چندضلعی‌های کوچک‌تر که روی زاویه‌های بیرونی چندضلعی اصلی متصل هستند.
    
    :param base_positions: لیست موقعیت‌ها و زوایای بیرونی چندضلعی اصلی
    :param sides: تعداد اضلاع چندضلعی کوچک‌تر
    :param length: طول اضلاع چندضلعی کوچک‌تر
    """
    for pos, heading in base_positions:
        turtle.penup()
        turtle.goto(pos)  # انتقال به رأس
        turtle.setheading(heading)  # تنظیم زاویه بیرونی
        turtle.pendown()

        # رسم چندضلعی کوچک‌تر
        angle = 360 / sides
        for _ in range(sides):
            turtle.forward(length)
            turtle.left(angle)

# بررسی تعداد اضلاع برای معتبر بودن
def validate_and_draw(main_sides, main_length, attached_sides, attached_length):
    """
    بررسی تعداد اضلاع و رسم چندضلعی اصلی و چندضلعی‌های متصل.
    
    :param main_sides: تعداد اضلاع چندضلعی اصلی
    :param main_length: طول اضلاع چندضلعی اصلی
    :param attached_sides: تعداد اضلاع چندضلعی متصل
    :param attached_length: طول اضلاع چندضلعی متصل
    """
    if main_sides <= 2 or attached_sides <= 2:
        print("خطا: تعداد اضلاع باید بیشتر از 2 باشد.")
        return

    # تنظیمات اولیه لاک‌پشت
    turtle.speed(5)
    turtle.pensize(2)

    # رسم چندضلعی اصلی
    main_positions = draw_polygon(main_sides, main_length)

    # رسم چندضلعی‌های متصل به زوایای بیرونی
    draw_attached_polygons(main_positions, attached_sides, attached_length)

    # اتمام برنامه با کلیک روی صفحه
    turtle.done()

# تعداد اضلاع و طول‌ها
main_sides = 6  # تعداد اضلاع چندضلعی اصلی
main_length = 100  # طول اضلاع چندضلعی اصلی
attached_sides = main_sides  # تعداد اضلاع چندضلعی متصل
attached_length = 100  # طول اضلاع چندضلعی متصل

# فراخوانی تابع بررسی و رسم
validate_and_draw(main_sides, main_length, attached_sides, attached_length)
